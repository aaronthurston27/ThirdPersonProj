// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_Tick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UAT_Tick : public UAbilityTask
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;

public:

	UAT_Tick(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_Tick* AbilityTask_OnTick(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;

private:
	float TaskDeltaTime;
};
