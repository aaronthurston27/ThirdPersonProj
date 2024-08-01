// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbility.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

void UTPPAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	CachedCharacterOwner = Cast<AThirdPersonProjCharacter>(ActorInfo->AvatarActor);
}

void UTPPAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UTPPAbility::IsAbilityInputPressed() const
{
	return GetCurrentAbilitySpec()->InputPressed;
}
