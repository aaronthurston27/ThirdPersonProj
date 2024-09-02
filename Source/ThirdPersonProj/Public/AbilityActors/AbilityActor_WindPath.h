// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "GameplayTagContainer.h"
#include "AbilityActor_WindPath.generated.h"

UCLASS()
class THIRDPERSONPROJ_API AAbilityActor_WindPath : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AAbilityActor_WindPath();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USplineComponent* PathSpline;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WindPathDuration = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float WindPathExpiredDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* CollisionStaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2"))
	uint8 WindCollisionSplinePointLength = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector WindCollisionStaticMeshScale = FVector::OneVector;

	UPROPERTY(Transient)
	int32 PreviousSplineCollisionMeshIndex = 0;

	UPROPERTY(Transient)
	FTimerHandle WindPathTimerHandle;

	UPROPERTY(Transient)
	bool bIsWindPathComplete = false;

	UFUNCTION(BlueprintNativeEvent)
	void OnWindPathDurationExpired();
	void OnWindPathDurationExpired_Implementation();

	void SpawnCollisionVolumeAtCurrentSplinePoint();

	UPROPERTY(Transient)
	TArray<UStaticMeshComponent*> WindCollisionMeshes;

	UPROPERTY(EditDefaultsOnly, Category = "Wind Force", meta = (UIMin = "1.0", ClampMin = "1.0"))
	float WindForceTowardsPath = 4000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Wind Force", meta = (UIMin = "1.0", ClampMin = "1.0"))
	float WindForceAlongPath = 4000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Wind Force")
	float MaxDistanceFromWindPath = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Wind Force")
	float GravityForceMultiplier = 5000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Wind Force")
	float NonInterfaceForceScalar = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wind Force")
	FGameplayTagContainer ForceTowardsPathContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Wind Force")
	FGameplayTagContainer ForceAlongPathContainer;

	void TickWindCollisionPhysics(float DeltaTime);

	bool ApplyWindForceToObject(UPrimitiveComponent* SourceComponent, UPrimitiveComponent* TargetComponent, const FVector& ClosestPointToSpline);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPointToSplinePath(const FVector& WorldLocation, const FVector& Direction);

	UFUNCTION(BlueprintNativeEvent)
	void OnSplinePointAdded(int32 SplineIndex);
	void OnSplinePointAdded_Index(int32 SplineIndex);

	UFUNCTION(BlueprintNativeEvent)
	void OnWindPathCompleted();
	void OnWindPathCompleted_Implementation();

};
