// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/AbilityForceTarget.h"

// Add default functionality here for any IAbilityForceTarget functions that are not pure virtual.

bool IAbilityForceTarget::CanApplyForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FName& BoneName, const FGameplayTag& ForceTag)
{
    return true;
}

void IAbilityForceTarget::AddForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FName& BoneName, const FGameplayTag& ForceTag)
{
}
