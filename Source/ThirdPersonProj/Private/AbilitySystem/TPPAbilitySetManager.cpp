// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TPPAbilitySetManager.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"
#include "AbilitySystem/TPPAbilitySet.h"
#include "Abilities/TPPAbility.h"
#include "AbilitySystem/TPPAbilitySystemComponent.h"

// Sets default values for this component's properties
UTPPAbilitySetManager::UTPPAbilitySetManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTPPAbilitySetManager::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<AThirdPersonProjCharacter>(GetOwner());
	if (CharacterOwner)
	{
		CachedAbilitySystem = CharacterOwner->GetTPPAbilitySystemComponent();
	}

	ensure(CharacterOwner);
	ensure(CachedAbilitySystem);

	if (AvailableAbilitySets.Num() > 0)
	{
		SetActiveAbilitySet(AvailableAbilitySets[0]);
	}
}


// Called every frame
void UTPPAbilitySetManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTPPAbilitySetManager::SetActiveAbilitySet(UTPPAbilitySet_Character* NewAbilitySet)
{
	if (!CachedAbilitySystem)
	{
		return;
	}

	if (ActiveAbilitySet)
	{
		TArray<FGameplayAbilitySpecHandle> AllHandles;
		AllHandles.Append(PrimaryAbilityHandles);
		AllHandles.Append(SecondaryAbilityHandles);
		AllHandles.Append(AuxiliaryAbilityHandles);
		AllHandles.Append(PassiveAbilityHandles);
		AllHandles.Add(UltimateAbilityHandle);
		AllHandles.Add(JumpAbilityHandle);
		AllHandles.Add(MeleeAbilityHandle);

		for (const FGameplayAbilitySpecHandle& SpecHandle : AllHandles)
		{
			CachedAbilitySystem->ClearAbility(SpecHandle);
		}

		PrimaryAbilityHandles.Empty();
		SecondaryAbilityHandles.Empty();
		AuxiliaryAbilityHandles.Empty();
		PassiveAbilityHandles.Empty();
		JumpAbilityHandle = FGameplayAbilitySpecHandle();
		MeleeAbilityHandle = FGameplayAbilitySpecHandle();
		UltimateAbilityHandle = FGameplayAbilitySpecHandle();
	}

	ActiveAbilitySet = NewAbilitySet;
	
	if (ActiveAbilitySet)
	{
		TArray<FGameplayAbilitySpecHandle> Passives;
		ActiveAbilitySet->GiveAbilities_ReturnHandles(CachedAbilitySystem, PrimaryAbilityHandles, SecondaryAbilityHandles, AuxiliaryAbilityHandles, Passives, JumpAbilityHandle, MeleeAbilityHandle, UltimateAbilityHandle);

		if (PrimaryAbilityHandles.Num() > 0)
		{
			SetActivePrimaryAbility(PrimaryAbilityHandles[0]);
		}

		if (SecondaryAbilityHandles.Num() > 0)
		{
			SetActiveSecondaryAbility(SecondaryAbilityHandles[0]);
		}

		if (AuxiliaryAbilityHandles.Num() > 0)
		{
			SetActiveAuxiliaryAbility(AuxiliaryAbilityHandles[0]);
		}
	}
}

void UTPPAbilitySetManager::SetActivePrimaryAbility(const FGameplayAbilitySpecHandle& SpecHandle)
{
	check(CachedAbilitySystem);

	if (!ensure(PrimaryAbilityHandles.Contains(SpecHandle)))
	{
		return;
	}

	if (ActivePrimaryAbilityHandle.IsValid())
	{
		UTPPAbility* CurrentPrimaryAbility = Cast<UTPPAbility>(CachedAbilitySystem->FindAbilitySpecFromHandle(ActivePrimaryAbilityHandle)->GetPrimaryInstance());
		if (CurrentPrimaryAbility)
		{
			CurrentPrimaryAbility->OnAbilityUnequipped();
		}
	}

	ActivePrimaryAbilityHandle = SpecHandle;

	const FGameplayAbilitySpec* PrimaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(SpecHandle);
	UTPPAbility* TPPAbility = PrimaryAbilitySpec ? Cast<UTPPAbility>(PrimaryAbilitySpec->GetPrimaryInstance()) : nullptr;
	if (TPPAbility)
	{
		TPPAbility->OnAbilityEquipped();

		if (TPPAbility->ShouldAbilityActivateOnEquip())
		{
			CachedAbilitySystem->TryActivateAbility(SpecHandle);
		}
	}
}

void UTPPAbilitySetManager::SetActiveSecondaryAbility(const FGameplayAbilitySpecHandle& SpecHandle)
{
	check(CachedAbilitySystem);

	if (!ensure(SecondaryAbilityHandles.Contains(SpecHandle)))
	{
		return;
	}

	if (ActiveSecondaryAbilityHandle.IsValid())
	{
		UTPPAbility* CurrentSecondaryAbility = Cast<UTPPAbility>(CachedAbilitySystem->FindAbilitySpecFromHandle(ActiveSecondaryAbilityHandle)->GetPrimaryInstance());
		if (CurrentSecondaryAbility)
		{
			CurrentSecondaryAbility->OnAbilityUnequipped();
		}
	}

	ActiveSecondaryAbilityHandle = SpecHandle;

	const FGameplayAbilitySpec* SecondaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(SpecHandle);
	UTPPAbility* TPPAbility = SecondaryAbilitySpec ? Cast<UTPPAbility>(SecondaryAbilitySpec->GetPrimaryInstance()) : nullptr;
	if (TPPAbility)
	{
		TPPAbility->OnAbilityEquipped();

		if (TPPAbility->ShouldAbilityActivateOnEquip())
		{
			CachedAbilitySystem->TryActivateAbility(SpecHandle);
		}
	}
}

void UTPPAbilitySetManager::SetActiveAuxiliaryAbility(const FGameplayAbilitySpecHandle& SpecHandle)
{
	check(CachedAbilitySystem);

	if (!ensure(AuxiliaryAbilityHandles.Contains(SpecHandle)))
	{
		return;
	}

	if (ActiveAuxiliaryAbilityHandle.IsValid())
	{
		UTPPAbility* CurrentAuxiliaryAbility = Cast<UTPPAbility>(CachedAbilitySystem->FindAbilitySpecFromHandle(ActiveAuxiliaryAbilityHandle)->GetPrimaryInstance());
		if (CurrentAuxiliaryAbility)
		{
			CurrentAuxiliaryAbility->OnAbilityUnequipped();
		}
	}

	ActiveAuxiliaryAbilityHandle = SpecHandle;

	const FGameplayAbilitySpec* AuxiliaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(SpecHandle);
	UTPPAbility* TPPAbility = AuxiliaryAbilitySpec ? Cast<UTPPAbility>(AuxiliaryAbilitySpec->GetPrimaryInstance()) : nullptr;
	if (TPPAbility)
	{
		TPPAbility->OnAbilityEquipped();

		if (TPPAbility->ShouldAbilityActivateOnEquip())
		{
			CachedAbilitySystem->TryActivateAbility(SpecHandle);
		}
	}
}

bool UTPPAbilitySetManager::IsPrimaryAbilityActive() const
{
	if (!CachedAbilitySystem)
	{
		return false;
	}

	const FGameplayAbilitySpec* PrimaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(ActivePrimaryAbilityHandle);
	return PrimaryAbilitySpec ? PrimaryAbilitySpec->IsActive() : false;
}

bool UTPPAbilitySetManager::IsSecondaryAbilityActive() const
{
	if (!CachedAbilitySystem)
	{
		return false;
	}

	const FGameplayAbilitySpec* SecondaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(ActiveSecondaryAbilityHandle);
	return SecondaryAbilitySpec ? SecondaryAbilitySpec->IsActive() : false;
}

bool UTPPAbilitySetManager::IsAuxiliaryAbilityActive() const
{
	if (!CachedAbilitySystem)
	{
		return false;
	}

	const FGameplayAbilitySpec* AuxiliaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(ActiveAuxiliaryAbilityHandle);
	return AuxiliaryAbilitySpec ? AuxiliaryAbilitySpec->IsActive() : false;
}

void UTPPAbilitySetManager::SelectNextPrimaryAbility()
{
	int32 CurrentPrimaryAbilityIndex = -1;
	CurrentPrimaryAbilityIndex = PrimaryAbilityHandles.Find(ActivePrimaryAbilityHandle);

	const int32 NextAbilityIndex = (CurrentPrimaryAbilityIndex + 1) % PrimaryAbilityHandles.Num();
	if (PrimaryAbilityHandles.IsValidIndex(NextAbilityIndex))
	{
		SetActivePrimaryAbility(PrimaryAbilityHandles[NextAbilityIndex]);
	}
}

void UTPPAbilitySetManager::SelectNextSecondaryAbility()
{
	int32 CurrentSecondaryAbilityIndex = -1;
	CurrentSecondaryAbilityIndex = SecondaryAbilityHandles.Find(ActiveSecondaryAbilityHandle);

	const int32 NextAbilityIndex = (CurrentSecondaryAbilityIndex + 1) % SecondaryAbilityHandles.Num();
	if (SecondaryAbilityHandles.IsValidIndex(NextAbilityIndex))
	{
		SetActiveSecondaryAbility(SecondaryAbilityHandles[NextAbilityIndex]);
	}
}

void UTPPAbilitySetManager::SelectNextAuxiliaryAbility()
{
	int32 CurrentAuxiliaryAbilityIndex = -1;
	CurrentAuxiliaryAbilityIndex = AuxiliaryAbilityHandles.Find(ActiveAuxiliaryAbilityHandle);

	const int32 NextAbilityIndex = (CurrentAuxiliaryAbilityIndex + 1) % AuxiliaryAbilityHandles.Num();
	if (AuxiliaryAbilityHandles.IsValidIndex(NextAbilityIndex))
	{
		SetActiveAuxiliaryAbility(AuxiliaryAbilityHandles[NextAbilityIndex]);
	}
}
