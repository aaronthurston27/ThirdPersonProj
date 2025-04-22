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
class THIRDPERSONPROJ_API UTPPEquipmentAbilitySet : public UTPPAbilitySet
{

public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo PrimaryAbility;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo SecondaryAbility;

	void GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, FGameplayAbilitySpecHandle& PrimaryAbilityHandle, FGameplayAbilitySpecHandle& SecondaryAbilityHandle);

};

/**
 *
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDPERSONPROJ_API UTPPEquipmentAbilitySet_Firearm : public UTPPEquipmentAbilitySet
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo ReloadAbility;

	void GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, FGameplayAbilitySpecHandle& PrimaryAbilityHandle, FGameplayAbilitySpecHandle& SecondaryAbilityHandle, FGameplayAbilitySpecHandle& ReloadHandle);
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
