// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActors/AbilityActor_WindPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAbilityActor_WindPath::AAbilityActor_WindPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	SetRootComponent(RootSceneComponent);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Path Spline"));
	PathSpline->SetupAttachment(RootSceneComponent);
}

// Called when the game starts or when spawned
void AAbilityActor_WindPath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbilityActor_WindPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickWindCollisionPhysics(DeltaTime);
}

void AAbilityActor_WindPath::AddPointToSplinePath(const FVector& WorldLocation, const FVector& Direction)
{
	PathSpline->AddSplinePoint(WorldLocation, ESplineCoordinateSpace::World);
	PathSpline->SetTangentAtSplinePoint(PathSpline->GetNumberOfSplinePoints() - 1, Direction, ESplineCoordinateSpace::World);

	//DrawDebugSphere(GetWorld(), WindPathActor->PathSpline->GetLocationAtSplinePoint(NewSplineIndex, ESplineCoordinateSpace::World), 25.0f, 4, FColor::Green, false, 8.0f, 0, .4f);

	OnSplinePointAdded(PathSpline->GetNumberOfSplinePoints() - 1);
}

void AAbilityActor_WindPath::OnSplinePointAdded_Implementation(int32 SplineIndex)
{
	if (SplineIndex - PreviousSplineCollisionMeshIndex >= WindCollisionSplinePointLength || SplineIndex == 0)
	{
		SpawnCollisionVolumeAtCurrentSplinePoint();
	}
}

void AAbilityActor_WindPath::OnWindPathCompleted_Implementation()
{
	bIsWindPathComplete = true;
	GetWorldTimerManager().SetTimer(WindPathTimerHandle, this, &AAbilityActor_WindPath::OnWindPathDurationExpired, WindPathDuration, false);
}

void AAbilityActor_WindPath::OnWindPathDurationExpired_Implementation()
{
	SetLifeSpan(WindPathExpiredDuration);
}

void AAbilityActor_WindPath::SpawnCollisionVolumeAtCurrentSplinePoint()
{
	int32 CurrentIndex = PathSpline->GetNumberOfSplinePoints() - 1;

	const FVector CurrentLocation = PathSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	const FVector TangentDir = PathSpline->GetTangentAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World).GetSafeNormal();
	const FRotator MeshRotation = UKismetMathLibrary::MakeRotFromZ(TangentDir);

	UStaticMeshComponent* CollisionStaticMeshSubobject = Cast<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, true));
	ensure(CollisionStaticMeshSubobject);
	if (CollisionStaticMeshSubobject)
	{
		CollisionStaticMeshSubobject->AttachToComponent(PathSpline, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FinishAddComponent(CollisionStaticMeshSubobject, true, FTransform::Identity);

		// Add collision events before enabling collision so that updates are correctly called.
		CollisionStaticMeshSubobject->OnComponentBeginOverlap.AddDynamic(this, &AAbilityActor_WindPath::OnWindMeshCollisionOverlapBegin);
		CollisionStaticMeshSubobject->OnComponentEndOverlap.AddDynamic(this, &AAbilityActor_WindPath::OnWindMeshCollisionOverlapEnd);

		CollisionStaticMeshSubobject->SetWorldLocationAndRotation(CurrentLocation, MeshRotation, false);
		CollisionStaticMeshSubobject->SetWorldScale3D(WindCollisionStaticMeshScale);
		CollisionStaticMeshSubobject->SetStaticMesh(CollisionStaticMesh);
		CollisionStaticMeshSubobject->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));

		WindCollisionMeshes.Add(CollisionStaticMeshSubobject);
	}

	PreviousSplineCollisionMeshIndex = CurrentIndex;
}

void AAbilityActor_WindPath::OnWindMeshCollisionOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && !OtherActor->IsA<APawn>() && OtherActor != this && !OtherActor->IsA<AAbilityActor_WindPath>())
	{
		if (OverlappedActors.Contains(OtherActor))
		{
			OverlappedActors[OtherActor]++;
		}
		else
		{
			OverlappedActors.Add(OtherActor, 1);
		}
	}
}

void AAbilityActor_WindPath::OnWindMeshCollisionOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappedActors.Contains(OtherActor))
	{
		--OverlappedActors[OtherActor];
		if (OverlappedActors[OtherActor] == 0)
		{
			OverlappedActors.Remove(OtherActor);
		}
	}
}

void AAbilityActor_WindPath::TickWindCollisionPhysics(float DeltaTime)
{
	for (auto& ActorKVP: OverlappedActors)
	{
		if (ActorKVP.Key)
		{
			const FVector ClosestPointOnSpline = PathSpline->FindLocationClosestToWorldLocation(ActorKVP.Key->GetActorLocation(), ESplineCoordinateSpace::World);

			FHitResult LOSHitResult;
			GetWorld()->LineTraceSingleByObjectType(LOSHitResult, ActorKVP.Key->GetActorLocation(), ClosestPointOnSpline, FCollisionObjectQueryParams::AllStaticObjects);
			if (!LOSHitResult.bBlockingHit)
			{
				ApplyWindForceToObject(DeltaTime, ActorKVP.Key, ClosestPointOnSpline);
			}
		}
	}
}

void AAbilityActor_WindPath::ApplyWindForceToObject(float DeltaTime, AActor* Actor, const FVector& ClosestPointToSpline)
{
	UMeshComponent* MeshComp = Actor->GetComponentByClass<UMeshComponent>();
	if (MeshComp && MeshComp->IsSimulatingPhysics())
	{
		//DrawDebugLine(GetWorld(), Actor->GetActorLocation(), ClosestPointToSpline, FColor::Green, false, 2.0f, 0, .4f);

		const FVector ForceTowardSplineVec = PathSpline->FindLocationClosestToWorldLocation(Actor->GetActorLocation(), ESplineCoordinateSpace::World) - Actor->GetActorLocation();
		const FVector WindDirectionAtSplinePoint = PathSpline->FindTangentClosestToWorldLocation(ClosestPointToSpline, ESplineCoordinateSpace::World).GetSafeNormal();
		const float DistanceFromPath = ForceTowardSplineVec.Size();
		const float WindPathDistanceRatio = FMath::Min(MaxDistanceFromWindPath, DistanceFromPath) / MaxDistanceFromWindPath;

		// Don't apply wind force toward the center of the path if this force will oppose the direction of the wind.
		const float TowardPath_WindDirectionDot = ForceTowardSplineVec.GetSafeNormal() | WindDirectionAtSplinePoint;
		static const float WindPathForceMinimumDot = -.2f;
		if (TowardPath_WindDirectionDot >= WindPathForceMinimumDot)
		{
			const float ForceMagnitudeTowardInnerPath = WindForceTowardsPath * WindPathDistanceRatio;
			MeshComp->AddImpulse(ForceTowardSplineVec.GetSafeNormal() * ForceMagnitudeTowardInnerPath * DeltaTime, NAME_None, false);
		}

		const float DistanceFromInnerWindPath = ForceTowardSplineVec.Size();
		const float WindForce = WindForceAlongPath * FMath::Max(.5f, 1.0f - WindPathDistanceRatio);
		// Apply some extra force in the direction of gravity if we're trying to move upward.
		const float GravityDot = FMath::Max(WindDirectionAtSplinePoint | FVector::UpVector, 0.0f);
		const FVector GravityForce = FVector::UpVector * GravityDot * GravityForce;
		// DrawDebugLine(GetWorld(), Actor->GetActorLocation(), Actor->GetActorLocation() + WindDirectionAtSplinePoint * 165.0f, FColor::Yellow, false, 5.0f, 0, .8f);
		MeshComp->AddImpulse((GravityForce + (WindDirectionAtSplinePoint * WindForce)) * DeltaTime, NAME_None, false);

	}
}

