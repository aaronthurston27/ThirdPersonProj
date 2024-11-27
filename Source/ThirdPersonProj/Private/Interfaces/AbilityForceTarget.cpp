// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/AbilityForceTarget.h"

// Add default functionality here for any IAbilityForceTarget functions that are not pure virtual.

bool IAbilityForceTarget::CanReceiveForce_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
    return true;
}

void IAbilityForceTarget::ReceiveForce_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
}

void IAbilityForceTarget::ReceiveImpulse_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FVector& ImpulseLocation, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{

}

void IAbilityForceTarget::ReceiveRadialImpulse_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Origin, const FVector& Radius, const float Strength)
{
}

void IAbilityForceTarget::ReceiveAngularImpulse_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
}

void IAbilityForceTarget::ReceiveTorque_Degrees_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName)
{
}
