// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/TPPEquipmentAbility.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"
#include "Items/TPPEquippableItem.h"

UTPPEquipmentAbility::UTPPEquipmentAbility(const FObjectInitializer& ObjectInitializer)
{
	bShouldAbilityRespondToInput = true;
}

void UTPPEquipmentAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
}

ATPPEquippableItem* UTPPEquipmentAbility::GetSourceEquippable() const
{
	return Cast<ATPPEquippableItem>(GetSourceObject(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo()));
}

bool UTPPEquipmentAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	ATPPEquippableItem* SourceEquippable = GetSourceEquippable();
	check(SourceEquippable);
	if (!SourceEquippable)
	{
		return false;
	}

	check(SourceEquippable->GetOwnerCharacter() == ActorInfo->AvatarActor);

	if (SourceEquippable->GetEquippableState() < EEquippableState::Equipped)
	{
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UTPPEquipmentAbilitySet::GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, FGameplayAbilitySpecHandle& PrimaryAbilityHandle, FGameplayAbilitySpecHandle& SecondaryAbilityHandle)
{
	check(AbilitySystemComponent);

	PrimaryAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(PrimaryAbility.GameplayAbilityClass, 1, (int32)PrimaryAbility.Command, SourceObject));
	SecondaryAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SecondaryAbility.GameplayAbilityClass, 1, (int32)SecondaryAbility.Command, SourceObject));
}

void UTPPEquipmentAbilitySet_Firearm::GiveAbilities_ReturnHandles(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject, FGameplayAbilitySpecHandle& PrimaryAbilityHandle, FGameplayAbilitySpecHandle& SecondaryAbilityHandle, FGameplayAbilitySpecHandle& ReloadHandle)
{
	check(AbilitySystemComponent);

	PrimaryAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(PrimaryAbility.GameplayAbilityClass, 1, (int32)PrimaryAbility.Command, SourceObject));
	SecondaryAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(SecondaryAbility.GameplayAbilityClass, 1, (int32)SecondaryAbility.Command, SourceObject));
	ReloadHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ReloadAbility.GameplayAbilityClass, 1, (int32)ReloadAbility.Command, SourceObject));
}
