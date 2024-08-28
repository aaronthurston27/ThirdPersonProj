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
class THIRDPERSONPROJ_API UTPPAbilitySet : public UGameplayAbilitySet
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray<FGameplayAbilityBindInfo> SecondaryAbilities;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo UltimateAbility;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	TArray < TSubclassOf < UGameplayAbility >> PassiveAbilities;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo JumpAbility;

	UPROPERTY(EditAnywhere, Category = AbilitySet)
	FGameplayAbilityBindInfo MeleeAbility;

	virtual void GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAbilitySpecHandle>& PrimarySpecHandles, TArray<FGameplayAbilitySpecHandle>& SecondarySpecHandles, TArray<FGameplayAbilitySpecHandle>& PassiveAbilityHandles, 
		FGameplayAbilitySpecHandle& JumpAbilityHandle, FGameplayAbilitySpecHandle& MeleeAbilityHandle, FGameplayAbilitySpecHandle& UltimateAbilityHandle) const;
	
};
