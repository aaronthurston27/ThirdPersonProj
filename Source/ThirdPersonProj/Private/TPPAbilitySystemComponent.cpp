// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbilitySystemComponent.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

void UTPPAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTPPAbilitySystemComponent::OnGameplayEffectAppliedWrapper);

	AddLooseGameplayTag(BaseGameplayTag);
}

void UTPPAbilitySystemComponent::BlockPrimaryAbilityInput()
{
	BlockAbilityByInputID(0);
}

void UTPPAbilitySystemComponent::UnblockPrimaryAbilityInput()
{
	UnBlockAbilityByInputID(0);
}

bool UTPPAbilitySystemComponent::IsPrimaryAbilityInputBlocked() const
{
	return IsAbilityInputBlocked(0);
}

void UTPPAbilitySystemComponent::BlockSecondaryAbilityInput()
{
	BlockAbilityByInputID(1);
}

void UTPPAbilitySystemComponent::UnblockSecondaryAbilityInput()
{
	UnBlockAbilityByInputID(1);
}

bool UTPPAbilitySystemComponent::IsSecondaryAbilityInputBlocked() const
{
	return IsAbilityInputBlocked(1);
}

void UTPPAbilitySystemComponent::CancelAbilitiesByTag(const FGameplayTagContainer& TagContainer)
{
	CancelAbilities(&TagContainer);
}

void UTPPAbilitySystemComponent::OnGameplayEffectAppliedWrapper(UAbilitySystemComponent* ABS, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	OnGameplayEffectApplied.Broadcast(ABS, EffectSpec, EffectHandle, EffectSpec.CapturedSourceTags.GetSpecTags());
}
