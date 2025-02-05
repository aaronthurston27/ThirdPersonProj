// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbilitySystemComponent.h"
#include "GameplayAbilitySet.h"
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

void UTPPAbilitySystemComponent::BlockPrimaryAbilityInput(const FName BlockReason)
{
	if (BlockReason == NAME_None)
	{
		return;
	}

	if (!PrimaryAbilityBlockReasons.Contains(BlockReason))
	{
		PrimaryAbilityBlockReasons.Add(BlockReason);
		BlockAbilityByInputID(0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::BlockPrimaryAbilityInput - Block reason %s already present."), *BlockReason.ToString());
	}
}

void UTPPAbilitySystemComponent::UnblockPrimaryAbilityInput(const FName BlockReason)
{
	if (PrimaryAbilityBlockReasons.Contains(BlockReason))
	{
		PrimaryAbilityBlockReasons.Remove(BlockReason);
		UnBlockAbilityByInputID(0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::UnblockPrimaryAbilityInput - Block reason %s already present."), *BlockReason.ToString());
	}
}

bool UTPPAbilitySystemComponent::IsPrimaryAbilityInputBlocked(const FName BlockReason) const
{
	return BlockReason == NAME_None ? IsAbilityInputBlocked(0) : PrimaryAbilityBlockReasons.Contains(BlockReason);
}

void UTPPAbilitySystemComponent::BlockSecondaryAbilityInput(const FName BlockReason)
{
	if (BlockReason.IsNone())
	{
		return;
	}

	if (!SecondaryAbilityBlockReasons.Contains(BlockReason))
	{
		SecondaryAbilityBlockReasons.Add(BlockReason);
		BlockAbilityByInputID(1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::BlockSecondaryAbilityInput - Block reason %s already present."), *BlockReason.ToString());
	}
}

void UTPPAbilitySystemComponent::UnblockSecondaryAbilityInput(const FName BlockReason)
{
	if (SecondaryAbilityBlockReasons.Contains(BlockReason))
	{
		SecondaryAbilityBlockReasons.Remove(BlockReason);
		UnBlockAbilityByInputID(1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::UnblockSecondaryAbilityInput - Block reason %s not present."), *BlockReason.ToString());
	}
}

bool UTPPAbilitySystemComponent::IsSecondaryAbilityInputBlocked(const FName BlockReason) const
{
	return BlockReason == NAME_None ? IsAbilityInputBlocked(1) : SecondaryAbilityBlockReasons.Contains(BlockReason);
}

void UTPPAbilitySystemComponent::BlockJumpAbilityInput(const FName BlockReason)
{
	if (BlockReason.IsNone())
	{
		return;
	}

	if (!JumpAbilityBlockReasons.Contains(BlockReason))
	{
		JumpAbilityBlockReasons.Add(BlockReason);
		BlockAbilityByInputID((int32)EGameplayAbilityInputBinds::Ability7);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::BlockJumpAbilityInput - Block reason %s already present."), *BlockReason.ToString());
	}
}

void UTPPAbilitySystemComponent::UnblockJumpAbilityInput(const FName BlockReason)
{
	if (JumpAbilityBlockReasons.Contains(BlockReason))
	{
		JumpAbilityBlockReasons.Remove(BlockReason);
		UnBlockAbilityByInputID((int32)EGameplayAbilityInputBinds::Ability7);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::UnblockJumpAbilityInput - Block reason %s not present."), *BlockReason.ToString());
	}
}

void UTPPAbilitySystemComponent::BlockUltimateAbilityInput(const FName BlockReason)
{
	if (BlockReason.IsNone())
	{
		return;
	}

	if (!UltimateAbilityBlockReasons.Contains(BlockReason))
	{
		UltimateAbilityBlockReasons.Add(BlockReason);
		BlockAbilityByInputID((int32)EGameplayAbilityInputBinds::Ability3);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::BlockUltimateAbilityInput - Block reason %s already present."), *BlockReason.ToString());
	}
}

void UTPPAbilitySystemComponent::UnblockUltimateAbilityInput(const FName BlockReason)
{
	if (UltimateAbilityBlockReasons.Contains(BlockReason))
	{
		UltimateAbilityBlockReasons.Remove(BlockReason);
		UnBlockAbilityByInputID((int32)EGameplayAbilityInputBinds::Ability3);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTPPAbilitySystemComponent::UnblockUltimateAbilityInput - Block reason %s not present."), *BlockReason.ToString());
	}
}

void UTPPAbilitySystemComponent::CancelAbilitiesByTag(const FGameplayTagContainer& TagContainer)
{
	CancelAbilities(&TagContainer);
}

void UTPPAbilitySystemComponent::ClearAbilities(const TArray<FGameplayAbilitySpecHandle>& Abilities)
{
	for (const FGameplayAbilitySpecHandle& SpecHandle : Abilities)
	{
		ClearAbility(SpecHandle);
	}
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
