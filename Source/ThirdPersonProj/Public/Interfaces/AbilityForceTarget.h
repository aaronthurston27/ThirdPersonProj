// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilityForceTarget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityForceTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THIRDPERSONPROJ_API IAbilityForceTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanApplyForceToTarget(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));
	
	bool CanApplyForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddForceToTarget(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	void AddForceToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddImpulseToTarget(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FVector& ImpulseLocation, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	void AddImpulseToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FVector& ImpulseLocation, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddRadialImpulseToTarget(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Origin, const FVector& Radius, const float Strength);

	void AddRadialImpulseToTarget_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Origin, const FVector& Radius, const float Strength);
};
