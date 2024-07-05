// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTasks/AT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UAT_PlayMontageAndWaitForEvent::UAT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UAT_PlayMontageAndWaitForEvent::Activate()
{
	if (AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			// Bind to event callback
			EventHandle = AbilitySystemComponent.Get()->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UAT_PlayMontageAndWaitForEvent::OnGameplayEvent));
		}
	}

	Super::Activate();
}

UAT_PlayMontageAndWaitForEvent* UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility_2, FName TaskInstanceName_2, UAnimMontage* MontageToPlay_2, FGameplayTagContainer TagContainer, float Rate_2, FName StartSection_2, bool bStopWhenAbilityEnds_2, float AnimRootMotionTranslationScale_2)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate_2);

	UAT_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<UAT_PlayMontageAndWaitForEvent>(OwningAbility_2, TaskInstanceName_2);
	MyObj->MontageToPlay = MontageToPlay_2;
	MyObj->Rate = Rate_2;
	MyObj->StartSection = StartSection_2;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale_2;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds_2;
	MyObj->EventTags = TagContainer;

	return MyObj;
}

void UAT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData TempData = *Payload;
		TempData.EventTag = EventTag;

		EventReceived.Broadcast(EventTag, TempData);
	}
}
