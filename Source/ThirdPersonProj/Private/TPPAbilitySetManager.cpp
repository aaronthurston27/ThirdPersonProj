// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbilitySetManager.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"
#include "TPPAbilitySet.h"
#include "TPPAbility.h"
#include "TPPAbilitySystemComponent.h"

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
		CachedAbilitySystem = CharacterOwner->GetAbilitySystemComponent();
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

void UTPPAbilitySetManager::SetActiveAbilitySet(UTPPAbilitySet* NewAbilitySet)
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
		AllHandles.Append(PassiveAbilityHandles);
		AllHandles.Add(JumpAbilityHandle);

		for (const FGameplayAbilitySpecHandle& SpecHandle : AllHandles)
		{
			CachedAbilitySystem->ClearAbility(SpecHandle);
		}

		PrimaryAbilityHandles.Empty();
		SecondaryAbilityHandles.Empty();
		PassiveAbilityHandles.Empty();
		JumpAbilityHandle = FGameplayAbilitySpecHandle();
	}

	ActiveAbilitySet = NewAbilitySet;
	
	if (ActiveAbilitySet)
	{
		TArray<FGameplayAbilitySpecHandle> Passives;
		ActiveAbilitySet->GiveAbilities_ReturnHandles(CachedAbilitySystem, PrimaryAbilityHandles, SecondaryAbilityHandles, Passives, JumpAbilityHandle);

		if (PrimaryAbilityHandles.Num() > 0)
		{
			SetActivePrimaryAbility(PrimaryAbilityHandles[0]);
		}

		if (SecondaryAbilityHandles.Num() > 0)
		{
			SetActiveSecondaryAbility(SecondaryAbilityHandles[0]);
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

	if (PrimaryActiveAbility.IsValid())
	{
		UTPPAbility* CurrentPrimaryAbility = Cast<UTPPAbility>(CachedAbilitySystem->FindAbilitySpecFromHandle(PrimaryActiveAbility)->GetPrimaryInstance());
		if (CurrentPrimaryAbility && !CurrentPrimaryAbility->bRespondToInputWhenInactive)
		{
			CurrentPrimaryAbility->SetAbilityInputResponse(false);
		}
	}

	PrimaryActiveAbility = SpecHandle;
	FGameplayAbilitySpec* PrimaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(SpecHandle);
	UTPPAbility* TPPAbility = PrimaryAbilitySpec ? Cast<UTPPAbility>(PrimaryAbilitySpec->Ability) : nullptr;
	if (TPPAbility)
	{
		if (TPPAbility->bShouldAbilityAutoActivate)
		{
			CachedAbilitySystem->TryActivateAbility(SpecHandle);
		}

		TPPAbility->SetAbilityInputResponse(true);
	}
}

void UTPPAbilitySetManager::SetActiveSecondaryAbility(const FGameplayAbilitySpecHandle& SpecHandle)
{
	check(CachedAbilitySystem);

	if (!ensure(SecondaryAbilityHandles.Contains(SpecHandle)))
	{
		return;
	}

	if (SecondaryActiveAbility.IsValid())
	{
		UTPPAbility* CurrentSecondaryAbility = Cast<UTPPAbility>(CachedAbilitySystem->FindAbilitySpecFromHandle(SecondaryActiveAbility)->GetPrimaryInstance());
		if (CurrentSecondaryAbility && !CurrentSecondaryAbility->bRespondToInputWhenInactive)
		{
			CurrentSecondaryAbility->SetAbilityInputResponse(false);
		}
	}

	SecondaryActiveAbility = SpecHandle;
	FGameplayAbilitySpec* SecondaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(SpecHandle);
	UTPPAbility* TPPAbility = SecondaryAbilitySpec ? Cast<UTPPAbility>(SecondaryAbilitySpec->Ability) : nullptr;
	if (TPPAbility)
	{
		if (TPPAbility->bShouldAbilityAutoActivate)
		{
			CachedAbilitySystem->TryActivateAbility(SpecHandle);
		}

		TPPAbility->SetAbilityInputResponse(true);
	}
}

bool UTPPAbilitySetManager::IsPrimaryAbilityActive() const
{
	if (!CachedAbilitySystem)
	{
		return false;
	}

	const FGameplayAbilitySpec* PrimaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(PrimaryActiveAbility);
	return PrimaryAbilitySpec ? PrimaryAbilitySpec->IsActive() : false;
}

bool UTPPAbilitySetManager::IsSecondaryAbilityActive() const
{
	if (!CachedAbilitySystem)
	{
		return false;
	}

	const FGameplayAbilitySpec* SecondaryAbilitySpec = CachedAbilitySystem->FindAbilitySpecFromHandle(SecondaryActiveAbility);
	return SecondaryAbilitySpec ? SecondaryAbilitySpec->IsActive() : false;
}

void UTPPAbilitySetManager::SelectNextPrimaryAbility()
{
	int32 CurrentPrimaryAbilityIndex = -1;
	CurrentPrimaryAbilityIndex = PrimaryAbilityHandles.Find(PrimaryActiveAbility);
	if (CurrentPrimaryAbilityIndex != -1)
	{
		const int32 NextAbilityIndex = (CurrentPrimaryAbilityIndex + 1) % PrimaryAbilityHandles.Num();
		if (PrimaryAbilityHandles.IsValidIndex(NextAbilityIndex))
		{
			SetActivePrimaryAbility(PrimaryAbilityHandles[NextAbilityIndex]);
		}
	}
}

void UTPPAbilitySetManager::SelectNextSecondaryAbility()
{
	int32 CurrentSecondaryAbilityIndex = -1;
	CurrentSecondaryAbilityIndex = SecondaryAbilityHandles.Find(SecondaryActiveAbility);
	if (CurrentSecondaryAbilityIndex != -1)
	{
		const int32 NextAbilityIndex = (CurrentSecondaryAbilityIndex + 1) % SecondaryAbilityHandles.Num();
		if (SecondaryAbilityHandles.IsValidIndex(NextAbilityIndex))
		{
			SetActiveSecondaryAbility(SecondaryAbilityHandles[NextAbilityIndex]);
		}
	}
}
