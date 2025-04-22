// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TPPEquippableItem_Firearm.h"
#include "Abilities/TPPEquipmentAbility.h"

void ATPPEquippableItem_Firearm::GrantEquippableAbilities()
{
	if (!ensureAlways(!bHasGrantedAbilties))
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComp = GetOwnerAbilitySystem();
	check(AbilitySystemComp);

	if (AbilitiesToGrant)
	{
		UTPPEquipmentAbilitySet_Firearm* AbilitySet = Cast<UTPPEquipmentAbilitySet_Firearm>(AbilitiesToGrant.LoadSynchronous());
		check(AbilitySet)

		AbilitySet->GiveAbilities_ReturnHandles(AbilitySystemComp, this, PrimaryAbilityHandle, SecondaryAbilityHandle, ReloadAbilityHandle);
	}

	bHasGrantedAbilties = true;
}

void ATPPEquippableItem_Firearm::RemoveEquippableAbilities()
{
	Super::RemoveEquippableAbilities();

	ReloadAbilityHandle = FGameplayAbilitySpecHandle();
}

TArray<FGameplayAbilitySpecHandle> ATPPEquippableItem_Firearm::GetGrantedAbilities() const
{
	TArray<FGameplayAbilitySpecHandle> Handles = Super::GetGrantedAbilities();
	Handles.Add(ReloadAbilityHandle);
	return Handles;
}
