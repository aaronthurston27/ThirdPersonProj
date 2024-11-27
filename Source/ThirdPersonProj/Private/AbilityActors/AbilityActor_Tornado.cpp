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

	FVector TangentialForceVector, CentripetalForceVector, UpwardForceVector, TorqueVector;
	for (UPrimitiveComponent* Component : OverlappingComponents)
	{
		if (VisitedActors.Contains(Component->GetOwner()))
		{
			continue;
		}

		IAbilityForceTarget* ForceTarget = Cast<IAbilityForceTarget>(Component->GetOwner());
		if (Component->GetOwner()->GetClass()->ImplementsInterface(UAbilityForceTarget::StaticClass()))
		{
			CalculateForceVectors(Component->GetOwner(), Component, DeltaTime, TangentialForceVector, CentripetalForceVector, UpwardForceVector, TorqueVector);
			VisitedActors.Add(Component->GetOwner());
			IAbilityForceTarget::Execute_ReceiveForce(Component->GetOwner(), this, TornadoCollisionMesh, Component, TangentialForceVector, TangentialForceGameplayTags, NAME_None);
			IAbilityForceTarget::Execute_ReceiveForce(Component->GetOwner(), this, TornadoCollisionMesh, Component, CentripetalForceVector, CentripetalForceGameplayTags, NAME_None);
			IAbilityForceTarget::Execute_ReceiveForce(Component->GetOwner(), this, TornadoCollisionMesh, Component, UpwardForceVector, UpwardForceGameplayTags, NAME_None);
			IAbilityForceTarget::Execute_ReceiveTorque_Degrees(Component->GetOwner(), this, TornadoCollisionMesh, Component, TorqueVector, TorqueForceGameplayTags, NAME_None);
		}
		else if (UMeshComponent* MeshComp = Cast<UMeshComponent>(Component))
		{
			if (MeshComp->IsSimulatingPhysics())
			{
				CalculateForceVectors(Component->GetOwner(), Component, DeltaTime, TangentialForceVector, CentripetalForceVector, UpwardForceVector, TorqueVector);
				const float MeshMass = MeshComp->GetMass();
				// Add mass to offset gravity acceleration.
				UpwardForceVector.Z *= MeshMass;
				VisitedActors.Add(Component->GetOwner());
				MeshComp->AddForce(TangentialForceVector * MeshMass);
				MeshComp->AddForce(CentripetalForceVector * MeshMass);
				MeshComp->AddForce(UpwardForceVector * NonInterfaceGravityScale, NAME_None, false);
				MeshComp->AddTorqueInDegrees(TorqueVector * MeshMass, NAME_None, true);
			}
		}	
	}
}

void AAbilityActor_Tornado::CalculateForceVectors(AActor* Actor, UPrimitiveComponent* Comp, float DeltaTime, FVector& TangentialForceVector, FVector& CentripetalForceVector, FVector& UpwardForceVector, FVector& Torque)
{
	check(Actor);
	check(Comp);
	if (!Actor || !Comp)
	{
		return;
	}

	const FVector ActorToTornadoCenterVec = Comp->GetComponentLocation() - TornadoCollisionMesh->GetComponentLocation();
	const float TornadoCenterDistanceRatio = FMath::Min(MaxDistanceFromTornadoCenter, ActorToTornadoCenterVec.Size2D()) / MaxDistanceFromTornadoCenter;

	float Radius, Height;
	TornadoCollisionMesh->CalcBoundingCylinder(Radius, Height);
	const FVector ClosestPointToEdge = TornadoCollisionMesh->GetComponentLocation() + (ActorToTornadoCenterVec.GetSafeNormal2D() * Radius);

	const FVector EdgeToCenterVec = ClosestPointToEdge - TornadoCollisionMesh->GetComponentLocation();
	const FVector TangentialVelocityVec = (FVector::UpVector ^ EdgeToCenterVec).GetSafeNormal();
	TangentialForceVector = TangentialVelocityVec * TangentialForce * FMath::Max(1.0f - TornadoCenterDistanceRatio, .2f);
	
	FHitResult LOSHitResult;
	const FVector TornadoCenterTraceEndLocation = FVector(TornadoCollisionMesh->GetComponentLocation().X, TornadoCollisionMesh->GetComponentLocation().Y, Comp->GetComponentLocation().Z);
	GetWorld()->LineTraceSingleByObjectType(LOSHitResult, Comp->GetComponentLocation(), TornadoCenterTraceEndLocation, FCollisionObjectQueryParams::AllStaticObjects);
	if (!LOSHitResult.bBlockingHit)
	{
		const FVector ComponentVelocity = Comp->GetComponentVelocity();
		const float Velocity_TangentDot = ComponentVelocity.GetSafeNormal2D() | TangentialVelocityVec;
		const FVector TangentialVelocity = ComponentVelocity * FMath::Max(0, Velocity_TangentDot);
		CentripetalForceVector = -ActorToTornadoCenterVec.GetSafeNormal2D() * TangentialVelocity.Size2D() * FMath::Max(.2f, TornadoCenterDistanceRatio) * CentripetalForceMultiplier;
	}
	else
	{
		CentripetalForceVector = FVector::ZeroVector;
	}

	const float GravityZ = -GetWorld()->GetGravityZ();
	UpwardForceVector = FVector::UpVector * (GravityZ + UpwardForce);

	Torque = FVector::UpVector * TorqueForceDegrees;
}

