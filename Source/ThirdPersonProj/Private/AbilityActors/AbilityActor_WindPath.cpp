// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActors/AbilityActor_WindPath.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/AbilityForceTarget.h"
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
	const int32 CurrentIndex = PathSpline->GetNumberOfSplinePoints() - 1;

	const FVector CurrentLocation = PathSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);
	const FVector TangentDir = PathSpline->GetTangentAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World).GetSafeNormal();
	const FRotator MeshRotation = UKismetMathLibrary::MakeRotFromZ(TangentDir);

	UStaticMeshComponent* CollisionStaticMeshSubobject = Cast<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, true));
	ensure(CollisionStaticMeshSubobject);
	if (CollisionStaticMeshSubobject)
	{
		CollisionStaticMeshSubobject->AttachToComponent(PathSpline, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FinishAddComponent(CollisionStaticMeshSubobject, true, FTransform::Identity);

		CollisionStaticMeshSubobject->SetWorldLocationAndRotation(CurrentLocation, MeshRotation, false);
		CollisionStaticMeshSubobject->SetWorldScale3D(WindCollisionStaticMeshScale);
		CollisionStaticMeshSubobject->SetStaticMesh(CollisionStaticMesh);
		CollisionStaticMeshSubobject->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));

		WindCollisionMeshes.Add(CollisionStaticMeshSubobject);
	}

	PreviousSplineCollisionMeshIndex = CurrentIndex;
}

void AAbilityActor_WindPath::TickWindCollisionPhysics(float DeltaTime)
{
	for (UPrimitiveComponent* WindCollisionComp : WindCollisionMeshes)
	{
		TSet<AActor*> AppliedForceActors;

		TArray<UPrimitiveComponent*> OverlappingComponents;
		WindCollisionComp->GetOverlappingComponents(OverlappingComponents);

		for (UPrimitiveComponent* OverlappingComp : OverlappingComponents)
		{
			if (AppliedForceActors.Contains(OverlappingComp->GetOwner()))
			{
				continue;
			}

			if (OverlappingComp->GetOwner() != this && OverlappingComp != nullptr)
			{
				const FVector ClosestPointOnSpline = PathSpline->FindLocationClosestToWorldLocation(OverlappingComp->GetComponentLocation(), ESplineCoordinateSpace::World);
				if (ApplyWindForceToObject(WindCollisionComp, OverlappingComp, ClosestPointOnSpline))
				{
					AppliedForceActors.Add(OverlappingComp->GetOwner());
				}
			}
		}
	}
}

bool AAbilityActor_WindPath::ApplyWindForceToObject(UPrimitiveComponent* SourceComponent, UPrimitiveComponent* TargetComponent, const FVector& ClosestPointToSpline)
{
	UMeshComponent* MeshComp = Cast<UMeshComponent>(TargetComponent);
	const bool bImplementsInterface = TargetComponent->GetOwner()->GetClass()->ImplementsInterface(UAbilityForceTarget::StaticClass());
	if (!bImplementsInterface)
	{
		if (!MeshComp || !MeshComp->IsSimulatingPhysics())
		{
			return false;
		}
	}
	else if (!IAbilityForceTarget::Execute_CanApplyForceToTarget(TargetComponent->GetOwner(), this, nullptr, TargetComponent, NAME_None, ForceTowardsPathContainer))
	{
		return false;
	}

	FHitResult LOSHitResult;
	GetWorld()->LineTraceSingleByObjectType(LOSHitResult, TargetComponent->GetComponentLocation(), ClosestPointToSpline, FCollisionObjectQueryParams::AllStaticObjects);
	if (LOSHitResult.bBlockingHit)
	{
		return false;
	}

	const FVector ForceTowardSplineVec = PathSpline->FindLocationClosestToWorldLocation(TargetComponent->GetComponentLocation(), ESplineCoordinateSpace::World) - TargetComponent->GetComponentLocation();
	const FVector WindDirectionAtSplinePoint = PathSpline->FindTangentClosestToWorldLocation(ClosestPointToSpline, ESplineCoordinateSpace::World).GetSafeNormal();
	const float DistanceFromPath = ForceTowardSplineVec.Size();
	const float WindPathDistanceRatio = FMath::Min(MaxDistanceFromWindPath, DistanceFromPath) / MaxDistanceFromWindPath;

	// Don't apply wind force toward the center of the path if this force will oppose the direction of the wind.
	const float TowardPath_WindDirectionDot = ForceTowardSplineVec.GetSafeNormal() | WindDirectionAtSplinePoint;
	static const float WindPathForceMinimumDot = -.2f;
	if (TowardPath_WindDirectionDot >= WindPathForceMinimumDot)
	{
		const float ForceMagnitudeTowardInnerPath = WindForceTowardsPath * WindPathDistanceRatio;
		FVector ForceToApply = ForceTowardSplineVec.GetSafeNormal() * ForceMagnitudeTowardInnerPath;
		if (bImplementsInterface)
		{
			IAbilityForceTarget::Execute_AddForceToTarget(TargetComponent->GetOwner(), this, nullptr, TargetComponent, ForceToApply, NAME_Name, ForceTowardsPathContainer);
		}
		else if (MeshComp)
		{
			ForceToApply *= NonInterfaceForceScalar;
			MeshComp->AddForce(ForceToApply, NAME_None, false);
		}
	}

	const float WindForce = WindForceAlongPath * FMath::Max(.5f, 1.0f - WindPathDistanceRatio) * (bImplementsInterface ? 1.0f : NonInterfaceForceScalar);
	// Apply some extra force in the direction of gravity if we're trying to move upward.
	const float GravityDot = FMath::Max(WindDirectionAtSplinePoint | FVector::UpVector, 0.0f);
	FVector GravityForce = FVector::UpVector * GravityDot * GravityForce;
	if (!bImplementsInterface)
	{
		GravityForce *= MeshComp->GetMass();
	}
	const FVector ForceToApply = (GravityForce + (WindDirectionAtSplinePoint * WindForce));

	if (bImplementsInterface)
	{
		IAbilityForceTarget::Execute_AddForceToTarget(TargetComponent->GetOwner(), this, nullptr, TargetComponent, ForceToApply, NAME_Name, ForceAlongPathContainer);
	}
	else if (MeshComp)
	{
		MeshComp->AddForce(ForceToApply, NAME_None, false);
	}

	return true;
}

