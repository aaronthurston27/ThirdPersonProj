// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AT_WaitConfirmInput.h"
#include "AbilitySystemComponent.h"

void UAT_WaitConfirmInput::OnConfirmCallback()
{
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::GenericConfirm, GetAbilitySpecHandle(), GetActivationPredictionKey());
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnConfirm.Broadcast();
		}
		EndTask();
	}
}

void UAT_WaitConfirmInput::OnLocalConfirmCallback()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get(), IsPredictingClient());

	if (AbilitySystemComponent.IsValid() && IsPredictingClient())
	{
		AbilitySystemComponent->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::GenericConfirm, GetAbilitySpecHandle(), GetActivationPredictionKey(), AbilitySystemComponent->ScopedPredictionKey);
	}
	OnConfirmCallback();
}

UAT_WaitConfirmInput* UAT_WaitConfirmInput::WaitConfirmInput(UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<UAT_WaitConfirmInput>(OwningAbility);
}

void UAT_WaitConfirmInput::Activate()
{
	if (AbilitySystemComponent.IsValid() && Ability)
	{
		const FGameplayAbilityActorInfo* Info = Ability->GetCurrentActorInfo();


		if (Info->IsLocallyControlled())
		{
			// We have to wait for the callback from the AbilitySystemComponent.
			AbilitySystemComponent->GenericLocalConfirmCallbacks.AddDynamic(this, &UAT_WaitConfirmInput::OnLocalConfirmCallback);	// Tell me if the confirm input is pressed

			Ability->OnWaitingForConfirmInputBegin();
		}
		else
		{
			if (CallOrAddReplicatedDelegate(EAbilityGenericReplicatedEvent::GenericConfirm, FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &UAT_WaitConfirmInput::OnConfirmCallback)))
			{
				// GenericConfirm was already received from the client and we just called OnConfirmCallback. The task is done.
				return;
			}
		}
	}
}

void UAT_WaitConfirmInput::OnDestroy(bool AbilityEnding)
{
	if (bHasRegisteredCallbacks && AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalConfirmCallbacks.RemoveDynamic(this, &UAT_WaitConfirmInput::OnLocalConfirmCallback);

		if (Ability)
		{
			Ability->OnWaitingForConfirmInputEnd();
		}
	}

	Super::OnDestroy(AbilityEnding);
}
