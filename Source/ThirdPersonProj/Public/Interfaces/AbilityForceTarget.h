// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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

	UFUNCTION(BlueprintNativeEvent)
	void TryAddForceToTarget(const AActor* ActorSource, const UActorComponent* SourceComponent, const FVector& Force, const FName& BoneName = FName(TEXT("None")));

	void TryAddForceToTarget_Implementation(const AActor* SourceActor, const UActorComponent* SourceComponent, const FVector& Force, const FName& BoneName);
};