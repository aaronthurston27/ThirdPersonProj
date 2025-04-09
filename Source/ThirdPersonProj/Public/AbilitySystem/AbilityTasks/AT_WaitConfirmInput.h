// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_WaitConfirmInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaitConfirmInputDelegate);

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UAT_WaitConfirmInput : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FWaitConfirmInputDelegate	OnConfirm;

	UFUNCTION()
	void OnConfirmCallback();

	UFUNCTION()
	void OnLocalConfirmCallback();

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true", DisplayName = "Wait for Confirm Input"), Category = "Ability|Tasks")
	static UAT_WaitConfirmInput* WaitConfirmInput(UGameplayAbility* OwningAbility);

	virtual void Activate() override;

protected:

	virtual void OnDestroy(bool AbilityEnding) override;

	bool bHasRegisteredCallbacks;
	
};
