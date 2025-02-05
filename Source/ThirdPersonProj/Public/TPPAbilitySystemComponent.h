// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "TPPAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayTagCountChanged, const FGameplayTag, GameplayTag, int32, Count);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBaseGameplayTagCountChanged, const FGameplayTag, GameplayTag, int32, Count);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class THIRDPERSONPROJ_API UTPPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void BlockPrimaryAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintCallable)
	void UnblockPrimaryAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintPure)
	bool IsPrimaryAbilityInputBlocked(const FName BlockReason = NAME_None) const;

	UFUNCTION(BlueprintCallable)
	void BlockSecondaryAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintCallable)
	void UnblockSecondaryAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintPure)
	bool IsSecondaryAbilityInputBlocked(const FName BlockReason = NAME_None) const;

	UFUNCTION(BlueprintCallable)
	void BlockJumpAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintCallable)
	void UnblockJumpAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintCallable)
	void BlockUltimateAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintCallable)
	void UnblockUltimateAbilityInput(const FName BlockReason);

	UFUNCTION(BlueprintCallable)
	void CancelAbilitiesByTag(const FGameplayTagContainer& TagContainer);

	void ClearAbilities(const TArray<FGameplayAbilitySpecHandle>& Abilities);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag BaseGameplayTag;

	UPROPERTY(BlueprintAssignable)
	FOnGameplayTagCountChanged OnAnyGameplayTagCountChanged;

	UPROPERTY(BlueprintAssignable)
	FOnBaseGameplayTagCountChanged OnBaseGameplayTagCountChanged;

	UPROPERTY(Transient)
	TSet<FName> PrimaryAbilityBlockReasons;

	UPROPERTY(Transient)
	TSet<FName> SecondaryAbilityBlockReasons;

	UPROPERTY(Transient)
	TSet<FName> UltimateAbilityBlockReasons;

	UPROPERTY(Transient)
	TSet<FName> JumpAbilityBlockReasons;

private:

	UFUNCTION()
	void HandleAnyGameplayTagCountChanged(const FGameplayTag Tag, int32 Count);

	UFUNCTION()
	void HandleBaseGameplayTagCountChanged(const FGameplayTag Tag, int32 Count);
};
