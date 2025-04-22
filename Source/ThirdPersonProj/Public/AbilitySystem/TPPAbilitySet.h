// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySet.h"
#include "TPPAbilitySet.generated.h"

class UTPPAbility;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class THIRDPERSONPROJ_API UTPPAbilitySet : public UDataAsset
{
	GENERATED_BODY()
};


UCLASS(Blueprintable, BlueprintType)
class THIRDPERSONPROJ_API UTPPAbilitySet_Character : public UTPPAbilitySet
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray<FGameplayAbilityBindInfo> PrimaryAbilities;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray<FGameplayAbilityBindInfo> SecondaryAbilities;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray<FGameplayAbilityBindInfo> AuxiliaryAbilities;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo UltimateAbility;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo JumpAbility;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo MeleeAbility;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray <TSubclassOf<UGameplayAbility>> PassiveAbilities;

	void GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAbilitySpecHandle>& PrimarySpecHandles, TArray<FGameplayAbilitySpecHandle>& SecondarySpecHandles, TArray<FGameplayAbilitySpecHandle>& AuxiliarySpecHandles, TArray<FGameplayAbilitySpecHandle>& PassiveAbilityHandles,
		FGameplayAbilitySpecHandle& JumpAbilityHandle, FGameplayAbilitySpecHandle& MeleeAbilityHandle, FGameplayAbilitySpecHandle& UltimateAbilityHandle) const;
};