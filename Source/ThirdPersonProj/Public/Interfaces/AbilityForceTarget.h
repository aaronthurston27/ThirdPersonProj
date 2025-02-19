// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilityForceTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTryAddForceToOwner, const UActorComponent*, SourceComponent, const FVector&, Force, const FGameplayTagContainer&, Tags, const FName, BoneName);

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
	bool CanReceiveForce(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));
	
	virtual bool CanReceiveForce_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveForce(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	virtual  void ReceiveForce_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveImpulse(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FVector& ImpulseLocation, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	virtual void ReceiveImpulse_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Force, const FVector& ImpulseLocation, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveRadialImpulse(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Origin, const FVector& Radius, const float Strength);

	virtual void ReceiveRadialImpulse_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Origin, const FVector& Radius, const float Strength);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveAngularImpulse(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	virtual void ReceiveAngularImpulse_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ReceiveTorque_Degrees(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));

	virtual void  ReceiveTorque_Degrees_Implementation(const AActor* ActorSource, const UActorComponent* SourceComponent, const UActorComponent* TargetComponent, const FVector& Impulse, const FGameplayTagContainer ForceTagContainer, const FName BoneName = FName(TEXT("None")));
};
