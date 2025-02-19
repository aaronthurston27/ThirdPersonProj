// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "TPPAbilitySetManager.generated.h"

class UTPPAbilitySet_Character;
class UTPPAbility;
class AThirdPersonProjCharacter;
class UTPPAbilitySystemComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class THIRDPERSONPROJ_API UTPPAbilitySetManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPPAbilitySetManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(Transient)
	AThirdPersonProjCharacter* CharacterOwner = nullptr;

	UPROPERTY(Transient)
	UTPPAbilitySystemComponent* CachedAbilitySystem = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UTPPAbilitySet_Character*> AvailableAbilitySets;

	UPROPERTY(Transient)
	UTPPAbilitySet_Character* ActiveAbilitySet = nullptr;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle ActivePrimaryAbilityHandle;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle ActiveSecondaryAbilityHandle;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle ActiveAuxiliaryAbilityHandle;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> PrimaryAbilityHandles;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> SecondaryAbilityHandles;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> AuxiliaryAbilityHandles;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> PassiveAbilityHandles;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle UltimateAbilityHandle;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle JumpAbilityHandle;

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle MeleeAbilityHandle;

public:

	UFUNCTION(BlueprintCallable)
	void SetActiveAbilitySet(UTPPAbilitySet_Character* NewAbilitySet);

	UFUNCTION(BlueprintCallable)
	void SetActivePrimaryAbility(const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintCallable)
	void SetActiveSecondaryAbility(const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintCallable)
	void SetActiveAuxiliaryAbility(const FGameplayAbilitySpecHandle& SpecHandle);

	UFUNCTION(BlueprintPure)
	bool IsPrimaryAbilityActive() const;

	UFUNCTION(BlueprintPure)
	bool IsSecondaryAbilityActive() const;

	UFUNCTION(BlueprintPure)
	bool IsAuxiliaryAbilityActive() const;

	void SelectNextPrimaryAbility();

	void SelectNextSecondaryAbility();

	void SelectNextAuxiliaryAbility();
		
};
