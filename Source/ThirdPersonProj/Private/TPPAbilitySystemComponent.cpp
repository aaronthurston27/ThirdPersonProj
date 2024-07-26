// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbilitySystemComponent.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

void UTPPAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BaseGameplayTag.IsValid())
	{
		AddLooseGameplayTag(BaseGameplayTag);
		RegisterGameplayTagEvent(BaseGameplayTag, EGameplayTagEventType::AnyCountChange).AddUObject(this, &UTPPAbilitySystemComponent::HandleBaseGameplayTagCountChanged);
	}

	RegisterGenericGameplayTagEvent().AddUObject(this, &UTPPAbilitySystemComponent::HandleAnyGameplayTagCountChanged);
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

void UTPPAbilitySystemComponent::HandleAnyGameplayTagCountChanged(const FGameplayTag Tag, int32 Count)
{
	if (!Tag.MatchesTag(BaseGameplayTag))
	{
		OnAnyGameplayTagCountChanged.Broadcast(Tag, Count);
	}
}

void UTPPAbilitySystemComponent::HandleBaseGameplayTagCountChanged(const FGameplayTag Tag, int32 Count)
{
	OnBaseGameplayTagCountChanged.Broadcast(Tag, Count);
}
