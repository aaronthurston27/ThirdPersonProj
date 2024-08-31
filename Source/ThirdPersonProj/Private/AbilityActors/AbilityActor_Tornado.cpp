// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActors/AbilityActor_Tornado.h"
#include "Interfaces/AbilityForceTarget.h"

// Sets default values
AAbilityActor_Tornado::AAbilityActor_Tornado()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TornadoCollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tornado Collision"));
	SetRootComponent(TornadoCollisionMesh);

}

// Called when the game starts or when spawned
void AAbilityActor_Tornado::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbilityActor_Tornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickTornadoCollisions(DeltaTime);
}

void AAbilityActor_Tornado::TickTornadoCollisions(float DeltaTime)
{
	TArray<UPrimitiveComponent*> OverlappingComponents;
	TArray<AActor*> VisitedActors;
	TornadoCollisionMesh->GetOverlappingComponents(OverlappingComponents);

	FVector TangentialForceVector, CentripetalForceVector;
	for (UPrimitiveComponent* Component : OverlappingComponents)
	{
		if (VisitedActors.Contains(Component->GetOwner()))
		{
			continue;
		}

		IAbilityForceTarget* ForceTarget = Cast<IAbilityForceTarget>(Component->GetOwner());
		if (Component->GetOwner()->GetClass()->ImplementsInterface(UAbilityForceTarget::StaticClass()))
		{
			if (IAbilityForceTarget::Execute_CanApplyForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, NAME_None, FGameplayTag::EmptyTag))
			{
				CalculateForceVectors(Component->GetOwner(), Component, DeltaTime, TangentialForceVector, CentripetalForceVector);
				VisitedActors.Add(Component->GetOwner());
				IAbilityForceTarget::Execute_AddForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, TangentialForceVector, NAME_None, FGameplayTag::EmptyTag);
				IAbilityForceTarget::Execute_AddForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, CentripetalForceVector, NAME_None, FGameplayTag::EmptyTag);
			}
		}
		else if (UMeshComponent* MeshComp = Cast<UMeshComponent>(Component))
		{
			if (MeshComp->IsSimulatingPhysics())
			{
				CalculateForceVectors(Component->GetOwner(), Component, DeltaTime, TangentialForceVector, CentripetalForceVector);
				VisitedActors.Add(Component->GetOwner());
				MeshComp->AddImpulse(TangentialForceVector);
				MeshComp->AddImpulse(CentripetalForceVector, NAME_None, false);
			}
		}	
	}
}

void AAbilityActor_Tornado::CalculateForceVectors(AActor* Actor, UPrimitiveComponent* Comp, float DeltaTime, FVector& TangentialForceVector, FVector& CentripetalForceVector)
{
	check(Actor);
	check(Comp);
	if (!Actor || !Comp)
	{
		return;
	}

	const FVector ActorToTornadoCenterVec = Actor->GetActorLocation() - TornadoCollisionMesh->GetComponentLocation();
	const float TornadoCenterDistanceRatio = FMath::Min(MaxDistanceFromTornadoCenter, ActorToTornadoCenterVec.Size2D()) / MaxDistanceFromTornadoCenter;
	
	FHitResult LOSHitResult;
	const FVector TornadoCenterTraceEndLocation = FVector(TornadoCollisionMesh->GetComponentLocation().X, TornadoCollisionMesh->GetComponentLocation().Y, Actor->GetActorLocation().Z);
	GetWorld()->LineTraceSingleByObjectType(LOSHitResult, Actor->GetActorLocation(), TornadoCenterTraceEndLocation, FCollisionObjectQueryParams::AllStaticObjects);
	if (!LOSHitResult.bBlockingHit)
	{
		CentripetalForceVector = -ActorToTornadoCenterVec.GetSafeNormal2D() * TangentialForce * TornadoCenterDistanceRatio * CentripetalForceMultiplier * DeltaTime;
	}
	else
	{
		CentripetalForceVector = FVector::ZeroVector;
	}

	float Radius, Height;
	TornadoCollisionMesh->CalcBoundingCylinder(Radius, Height);
	const FVector ClosestPointToEdge = TornadoCollisionMesh->GetComponentLocation() + (ActorToTornadoCenterVec.GetSafeNormal2D() * Radius);
	//DrawDebugLine(GetWorld(), ClosestPointToEdge, ClosestPointToEdge + TangentialForceVector.GetSafeNormal2D() * 200.0f, FColor::Red, false, 8.0f, 0, .8f);

	const FVector EdgeToCenterVec = ClosestPointToEdge - TornadoCollisionMesh->GetComponentLocation();
	const FVector TangentialVelocityVec = (EdgeToCenterVec ^ FVector::UpVector).GetSafeNormal();

	TangentialForceVector = TangentialVelocityVec.GetSafeNormal() * TangentialForce * FMath::Max(1.0f - TornadoCenterDistanceRatio, .2f);
	TangentialForceVector += FVector::UpVector * UpwardForce;
	TangentialForceVector *= DeltaTime;
}

