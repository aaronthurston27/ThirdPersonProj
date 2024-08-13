// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "AbilityTargetActor_WindPaint.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API AAbilityTargetActor_WindPaint : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_BODY()

	virtual void StartTargeting(UGameplayAbility* InAbility) override;

	virtual void Tick(float DeltaSeconds) override;

protected:

	/** Amount to change trace distance by when responding to input */
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistanceModulationValue = 25.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace", meta = (ClampMin = "0", UIMin = "0"))
	float MinimumTraceDistance = 250.0f;

	UPROPERTY(Transient)
	FVector WindDirection_LocalSpace = FVector::ForwardVector;

	UPROPERTY(Transient)
	float CachedMaxRange = 0.0f;

	void OnAbilityScrollAxisValueChanged(float AxisValue);

	virtual FHitResult PerformTrace(AActor* InSourceActor) override;

	virtual bool IsConfirmTargetingAllowed() override;

	UPROPERTY(Transient)
	bool bWasLastTraceValid = false;


public:

	UFUNCTION(BlueprintCallable)
	void SetWindDirectionLocalSpace(const FVector& WindDirectionLocalSpace);

	virtual void ConfirmTargetingAndContinue() override;
	
};
