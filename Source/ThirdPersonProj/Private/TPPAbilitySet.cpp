// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbilitySet.h"
#include "AbilitySystemComponent.h"

void UTPPAbilitySet::GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAbilitySpecHandle>& PrimarySpecHandles, TArray<FGameplayAbilitySpecHandle>& SecondarySpecHandles) const
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
}