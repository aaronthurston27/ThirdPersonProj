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
}

void AAbilityActor_WindPath::AddPointToSplinePath(const FVector& WorldLocation, const FVector& Direction)
{
	PathSpline->AddSplinePoint(WorldLocation, ESplineCoordinateSpace::World);
	PathSpline->SetTangentAtSplinePoint(PathSpline->GetSplineLength(), Direction, ESplineCoordinateSpace::World);

	//DrawDebugSphere(GetWorld(), WindPathActor->PathSpline->GetLocationAtSplinePoint(NewSplineIndex, ESplineCoordinateSpace::World), 25.0f, 4, FColor::Green, false, 8.0f, 0, .4f);

	OnSplinePointAdded(PathSpline->GetSplineLength());
}

void AAbilityActor_WindPath::OnSplinePointAdded_Implementation(int32 SplineIndex)
{
	if (SplineIndex - PreviousSplineCollisionMeshIndex >= WindCollisionSplinePointLength)
	{
		SpawnCollisionVolumeBetweenSplinePoints();
	}
}

void AAbilityActor_WindPath::OnWindPathCompleted_Implementation()
{
	GetWorldTimerManager().SetTimer(WindPathTimerHandle, this, &AAbilityActor_WindPath::OnWindPathDurationExpired, WindPathDuration, false);
}

void AAbilityActor_WindPath::OnWindPathDurationExpired_Implementation()
{
	SetLifeSpan(WindPathExpiredDuration);
}

void AAbilityActor_WindPath::SpawnCollisionVolumeBetweenSplinePoints()
{
	int32 CurrentIndex = PathSpline->GetSplineLength();
	ensure(CurrentIndex != PreviousSplineCollisionMeshIndex);

	const FVector FirstLocation = PathSpline->GetLocationAtSplinePoint(PreviousSplineCollisionMeshIndex, ESplineCoordinateSpace::World);
	const FVector SecondLocation = PathSpline->GetLocationAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World);

	const FVector CenterPoint = (FirstLocation + SecondLocation) * .5f;
	const FVector TangentDir = PathSpline->GetTangentAtSplinePoint(CurrentIndex, ESplineCoordinateSpace::World).GetSafeNormal();
	const FRotator MeshRotation = UKismetMathLibrary::MakeRotFromZ(TangentDir);

	UStaticMeshComponent* CollisionStaticMeshSubobject = Cast<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(), false, FTransform::Identity, true));
	ensure(CollisionStaticMeshSubobject);
	if (CollisionStaticMeshSubobject)
	{
		CollisionStaticMeshSubobject->AttachToComponent(PathSpline, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FinishAddComponent(CollisionStaticMeshSubobject, true, FTransform::Identity);

		CollisionStaticMeshSubobject->SetWorldLocationAndRotation(CenterPoint, MeshRotation, false);
		CollisionStaticMeshSubobject->SetWorldScale3D(WindCollisionStaticMeshScale);
		CollisionStaticMeshSubobject->SetStaticMesh(CollisionStaticMesh);
		CollisionStaticMeshSubobject->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	PreviousSplineCollisionMeshIndex = CurrentIndex;
}

