// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbility.h"
#include "TPPMovementComponent.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

UTPPAbility::UTPPAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTPPAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	CachedCharacterOwner = Cast<AThirdPersonProjCharacter>(ActorInfo->AvatarActor);
	if (!bRespondToInputWhenInactive)
	{
		bShouldAbilityRespondToInput = false;
	}
}

void UTPPAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UTPPAbility::IsAbilityInputPressed() const
{
	return GetCurrentAbilitySpec()->InputPressed;
}

void UTPPAbility::OnAbilityEquipped_Implementation()
{
	SetAbilityInputResponse(true);
}

void UTPPAbility::OnAbilityUnequipped_Implementation()
{
	if (ShouldCancelAbilityOnUnequip())
	{
		CancelAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true);
	}

	if (!ShouldAbilityRespondToInputWhenInactive())
	{
		SetAbilityInputResponse(false);
	}
}

void UTPPAbility::SetAbilityInputResponse(bool bShouldRespondToInput)
{
	bShouldAbilityRespondToInput = bShouldRespondToInput;
}

bool UTPPAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!bShouldAbilityRespondToInput && !bRespondToInputWhenInactive)
	{
		return false;
	}

	if (!bCanActivateWhileAirborne && CachedCharacterOwner->GetThirdPersonMovementComponent()->IsFalling())
	{
		return false;
	}

	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}
