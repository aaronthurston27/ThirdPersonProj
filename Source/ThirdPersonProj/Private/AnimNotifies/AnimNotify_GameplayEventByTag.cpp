// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AnimNotify_GameplayEventByTag.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAnimNotify_GameplayEventByTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Owner = Cast<AActor>(MeshComp->GetOwner());
	if (Owner)
	{
		FGameplayEventData NotifyEventData;
		NotifyEventData.EventTag = GameplayTagToTrigger;
		NotifyEventData.Instigator = Owner;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, GameplayTagToTrigger, NotifyEventData);
	}
}
