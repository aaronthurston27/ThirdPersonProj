// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/AbilityForceTarget.h"

// Add default functionality here for any IAbilityForceTarget functions that are not pure virtual.

bool IAbilityForceTarget::CanApplyForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
    return true;
}

void IAbilityForceTarget::AddForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
}

void IAbilityForceTarget::AddImpulseToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FVector& ImpulseLocation, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{

}

void IAbilityForceTarget::AddRadialImpulseToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Origin, const FVector& Radius, const float Strength)
{
}

void IAbilityForceTarget::AddAngularImpulseToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
}

void IAbilityForceTarget::AddTorqueToTarget_Degrees_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
}
