// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
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

	UFUNCTION(BlueprintNativeEvent)
	void OnWindPathDurationExpired();
	void OnWindPathDurationExpired_Implementation();

	void SpawnCollisionVolumeBetweenSplinePoints();

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
