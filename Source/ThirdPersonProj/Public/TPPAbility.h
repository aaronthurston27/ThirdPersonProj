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

	void SetAbilityInputResponse(bool bShouldRespondToInput = false);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldAbilityAutoActivate = false;

	UPROPERTY(EditDefaultsOnly)
	bool bCancelAbilityOnAbilityTagBlock = false;

	UPROPERTY(EditDefaultsOnly)
	bool bRespondToInputWhenInactive = false;

	UPROPERTY(EditDefaultsOnly)
	bool bCancelAbilityOnUnequip = true;

	UPROPERTY(Transient)
	bool bShouldAbilityRespondToInput = true;

	UPROPERTY(Transient, BlueprintReadOnly)
	AThirdPersonProjCharacter* CachedCharacterOwner = nullptr;

public:

	UFUNCTION(BlueprintPure)
	bool ShouldAbilityActivateOnEquip() const { return bShouldAbilityAutoActivate; }

	UFUNCTION(BlueprintPure)
	bool ShouldCancelAbilityOnUnequip() const { return bCancelAbilityOnUnequip;  }

	UFUNCTION(BlueprintPure)
	bool ShouldAbilityRespondToInputWhenInactive() const { return bRespondToInputWhenInactive; }

	UFUNCTION(BlueprintPure)
	bool IsAbilityInputPressed() const;

	UFUNCTION(BlueprintNativeEvent)
	void OnAbilityEquipped();

	void OnAbilityEquipped_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnAbilityUnequipped();

	void OnAbilityUnequipped_Implementation();

};
