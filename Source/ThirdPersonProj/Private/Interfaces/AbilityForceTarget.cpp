// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/AbilityForceTarget.h"

// Add default functionality here for any IAbilityForceTarget functions that are not pure virtual.

bool IAbilityForceTarget::CanApplyForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent)
{
    return true;
}

void IAbilityForceTarget::TryAddForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const FVector& Force, const FName& BoneName)
{
}
