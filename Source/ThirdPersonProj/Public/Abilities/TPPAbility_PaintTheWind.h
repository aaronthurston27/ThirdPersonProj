// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPPAbility.h"
#include "TPPAbility_PaintTheWind.generated.h"

class AAbilityActor_WindPath;

USTRUCT(BlueprintType)
struct FGameplayAbilityTargetData_WindPaint : public FGameplayAbilityTargetData
{

	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector StartingPosition = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector PathCenterPoint = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector WindTravelDirection = FVector::ZeroVector;
};

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UTPPAbility_PaintTheWind : public UTPPAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxLengthFromCenterPoint = 700.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float PathDrawingSpeed = 75.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRotator PathRotationSpeed = FRotator(30.0f, 30.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly, meta = (UIMax = "89.0", ClampMax = "89.0", UIMin = "0", ClampMin = "0"))
	float MaxPathPitch = 90.0f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityActor_WindPath> WindPathClass;

	UPROPERTY(Transient)
	AAbilityActor_WindPath* WindPathActor = nullptr;

	UPROPERTY(Transient)
	FVector CurrentPathLocation = FVector::ZeroVector;	

	UPROPERTY(Transient)
	FVector CurrentPathDirection = FVector::ZeroVector;

	UPROPERTY(Transient)
	FVector OriginalPathDirection = FVector::ZeroVector;

	UPROPERTY(Transient)
	FVector PathCenterPoint = FVector::ZeroVector;

	UPROPERTY(Transient)
	bool bPathCenterPointReached = false;

	UFUNCTION(BlueprintCallable)
	void UpdateWindPath(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void CreateWindPath(const FVector& WindPathStartingPoint, const FVector& WindPathCenterPoint, const FRotator& Rotation);

	void AdjustWindDirection(float DeltaTime);

	FVector GetTargetWindDirection() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnWindPathActorCreated();
	void OnWindPathActorCreated_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnWindPathCompleted();
	void OnWindPathCompleted_Implementation();

	UFUNCTION(BlueprintPure)
	FGameplayAbilityTargetData_WindPaint GetTargetDataFromHandle(const FGameplayAbilityTargetDataHandle& Handle) const;
};
