// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_GameplayEventByTag.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UAnimNotify_GameplayEventByTag : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag GameplayTagToTrigger;

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
