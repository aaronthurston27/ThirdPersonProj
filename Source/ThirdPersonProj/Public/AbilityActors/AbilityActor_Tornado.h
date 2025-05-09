// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AbilityActor_Tornado.generated.h"

UCLASS()
class THIRDPERSONPROJ_API AAbilityActor_Tornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityActor_Tornado();

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* TornadoCollisionMesh;

	UPROPERTY(EditDefaultsOnly)
	float TangentialForce = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer TangentialForceGameplayTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = "1.0", ClampMin = "1.0"))
	float CentripetalForceMultiplier = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer CentripetalForceGameplayTags;

	UPROPERTY(EditDefaultsOnly)
	float MaxDistanceFromTornadoCenter = 150.0f;

	UPROPERTY(EditDefaultsOnly)
	float UpwardForce = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float NonInterfaceGravityScale = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer UpwardForceGameplayTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TorqueForceDegrees = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer TorqueForceGameplayTags;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TickTornadoCollisions(float DeltaTime);

	void CalculateForceVectors(AActor* Actor, UPrimitiveComponent* Comp, float DeltaTime, FVector& TangentialForceVector, FVector& CentripetalForce, FVector& UpwardForce, FVector& Torque);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
