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

	FVector TangentialForceVector, CentripetalForceVector, UpwardForceVector;
	for (UPrimitiveComponent* Component : OverlappingComponents)
	{
		if (VisitedActors.Contains(Component->GetOwner()))
		{
			continue;
		}

		IAbilityForceTarget* ForceTarget = Cast<IAbilityForceTarget>(Component->GetOwner());
		if (Component->GetOwner()->GetClass()->ImplementsInterface(UAbilityForceTarget::StaticClass()))
		{
			if (IAbilityForceTarget::Execute_CanApplyForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, NAME_None, FGameplayTagContainer::EmptyContainer))
			{
				CalculateForceVectors(Component->GetOwner(), Component, DeltaTime, TangentialForceVector, CentripetalForceVector, UpwardForceVector);
				VisitedActors.Add(Component->GetOwner());
				IAbilityForceTarget::Execute_AddForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, TangentialForceVector, NAME_None, TangentialForceGameplayTags);
				IAbilityForceTarget::Execute_AddForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, CentripetalForceVector, NAME_None, CentripetalForceGameplayTags);
				IAbilityForceTarget::Execute_AddForceToTarget(Component->GetOwner(), this, TornadoCollisionMesh, Component, UpwardForceVector, NAME_None, UpwardForceGameplayTags);
			}
		}
		else if (UMeshComponent* MeshComp = Cast<UMeshComponent>(Component))
		{
			if (MeshComp->IsSimulatingPhysics())
			{
				CalculateForceVectors(Component->GetOwner(), Component, DeltaTime, TangentialForceVector, CentripetalForceVector, UpwardForceVector);
				// Add mass to offset gravity acceleration.
				UpwardForceVector.Z *= MeshComp->GetMass();
				VisitedActors.Add(Component->GetOwner());
				MeshComp->AddForce(TangentialForceVector * FMath::Max(1.0f, MeshComp->GetMass() * TangentialForceMassPercentage));
				MeshComp->AddForce(CentripetalForceVector * FMath::Max(1.0f, MeshComp->GetMass() * CentripetalForceMassPercentage));
				MeshComp->AddForce(UpwardForceVector * NonInterfaceGravityScale, NAME_None, false);
			}
		}	
	}
}

void AAbilityActor_Tornado::CalculateForceVectors(AActor* Actor, UPrimitiveComponent* Comp, float DeltaTime, FVector& TangentialForceVector, FVector& CentripetalForceVector, FVector& UpwardForceVector)
{
	check(Actor);
	check(Comp);
	if (!Actor || !Comp)
	{
		return;
	}

	const FVector ActorToTornadoCenterVec = Comp->GetComponentLocation() - TornadoCollisionMesh->GetComponentLocation();
	const float TornadoCenterDistanceRatio = FMath::Min(MaxDistanceFromTornadoCenter, ActorToTornadoCenterVec.Size2D()) / MaxDistanceFromTornadoCenter;
	
	FHitResult LOSHitResult;
	const FVector TornadoCenterTraceEndLocation = FVector(TornadoCollisionMesh->GetComponentLocation().X, TornadoCollisionMesh->GetComponentLocation().Y, Comp->GetComponentLocation().Z);
	GetWorld()->LineTraceSingleByObjectType(LOSHitResult, Comp->GetComponentLocation(), TornadoCenterTraceEndLocation, FCollisionObjectQueryParams::AllStaticObjects);
	if (!LOSHitResult.bBlockingHit)
	{
		CentripetalForceVector = -ActorToTornadoCenterVec.GetSafeNormal2D() * TangentialForce * TornadoCenterDistanceRatio * CentripetalForceMultiplier;
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
	const FVector TangentialVelocityVec = (FVector::UpVector ^ EdgeToCenterVec).GetSafeNormal();
	DrawDebugLine(GetWorld(), ClosestPointToEdge, ClosestPointToEdge + (TangentialVelocityVec * 90.0f), FColor::Red, false, 9.0f, 0, .8f);
	TangentialForceVector = TangentialVelocityVec * TangentialForce * FMath::Max(1.0f - TornadoCenterDistanceRatio, .2f);

	const float GravityZ = -GetWorld()->GetGravityZ();
	UpwardForceVector = FVector::UpVector * (GravityZ + UpwardForce);
}

