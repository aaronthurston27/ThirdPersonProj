// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/TPPAbility.h"
#include "AbilitySystem/TPPAbilitySet.h"
#include "TPPEquipmentAbility.generated.h"

class ATPPEquippableItem;

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDPERSONPROJ_API UTPPAbilitySet_Equipment : public UTPPAbilitySet
{
	GENERATED_BODY()
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class THIRDPERSONPROJ_API UTPPEquipmentAbility : public UTPPAbility
{
	GENERATED_BODY()

public:

	UTPPEquipmentAbility(const FObjectInitializer& ObjectInitializer);

	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	UFUNCTION(BlueprintPure)
	ATPPEquippableItem* GetSourceEquippable() const;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	
};
