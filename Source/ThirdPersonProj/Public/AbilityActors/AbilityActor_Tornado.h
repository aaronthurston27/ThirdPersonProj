// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityActor_Tornado.generated.h"

UCLASS()
class THIRDPERSONPROJ_API AAbilityActor_Tornado : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityActor_Tornado();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* TornadoCollisionMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TangentialForce = 1500.0f;

	UPROPERTY(EditDefaultsOnly)
	float UpwardForce = 1500.0f;

	UPROPERTY(EditDefaultsOnly)
	float CentripetalForceMultiplier = 1500.0f;

	UPROPERTY(EditDefaultsOnly)
	float MaxDistanceFromTornadoCenter = 150.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TickTornadoCollisions(float DeltaTime);

	void CalculateForceVectors(AActor* Actor, UPrimitiveComponent* Comp, float DeltaTime, FVector& TangentialForceVector, FVector& CentripetalForce);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
