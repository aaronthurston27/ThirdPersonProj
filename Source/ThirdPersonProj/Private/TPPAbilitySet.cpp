// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbilitySet.h"
#include "AbilitySystemComponent.h"

void UTPPAbilitySet::GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAbilitySpecHandle>& PrimarySpecHandles, TArray<FGameplayAbilitySpecHandle>& SecondarySpecHandles, 
	TArray<FGameplayAbilitySpecHandle>& PassiveAbilityHandles, FGameplayAbilitySpecHandle& JumpAbilityHandle, FGameplayAbilitySpecHandle& MeleeAbilityHandle, FGameplayAbilitySpecHandle& UltimateAbilityHandle) const
{
	for (const FGameplayAbilityBindInfo& BindInfo : Abilities)
	{
		if (BindInfo.GameplayAbilityClass)
		{
			const FGameplayAbilitySpecHandle PrimaryHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.GameplayAbilityClass, 1, (int32)BindInfo.Command));
			if (PrimaryHandle.IsValid())
			{
				PrimarySpecHandles.Add(PrimaryHandle);
			}
		}
	}

	for (const FGameplayAbilityBindInfo& BindInfo : SecondaryAbilities)
	{
		if (BindInfo.GameplayAbilityClass)
		{
			const FGameplayAbilitySpecHandle SecondaryHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(BindInfo.GameplayAbilityClass, 1, (int32)BindInfo.Command));
			if (SecondaryHandle.IsValid())
			{
				SecondarySpecHandles.Add(SecondaryHandle);
			}
		}
	}

	for (const TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilities)
	{
		if (AbilityClass)
		{
			const FGameplayAbilitySpecHandle PassiveHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, 0));
			if (PassiveHandle.IsValid())
			{
				PassiveAbilityHandles.Add(PassiveHandle);
			}
		}
	}

	if (UltimateAbility.GameplayAbilityClass)
	{
		UltimateAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UltimateAbility.GameplayAbilityClass, 1, (int32)EGameplayAbilityInputBinds::Ability3));
	}

	if (JumpAbility.GameplayAbilityClass)
	{
		// Ability 7 is jump.
		JumpAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(JumpAbility.GameplayAbilityClass, 1, (int32)EGameplayAbilityInputBinds::Ability7));
	}

	if (MeleeAbility.GameplayAbilityClass)
	{
		// Ability 5 is melee.
		JumpAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(MeleeAbility.GameplayAbilityClass, 1, (int32)EGameplayAbilityInputBinds::Ability5));
	}
}
