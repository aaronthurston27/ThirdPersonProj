// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPPAbility.generated.h"

class AThirdPersonProjCharacter;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class THIRDPERSONPROJ_API UTPPAbility : public UGameplayAbility
{
	GENERATED_BODY()

	UTPPAbility();

	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldAbilityAutoActivate = false;

	UPROPERTY(EditDefaultsOnly)
	bool bCancelAbilityOnAbilityTagBlock = false;

	UFUNCTION(BlueprintPure)
	bool IsAbilityInputPressed() const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly)
	AThirdPersonProjCharacter* CachedCharacterOwner = nullptr;

};
