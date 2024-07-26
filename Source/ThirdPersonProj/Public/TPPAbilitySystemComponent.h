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
	void BlockPrimaryAbilityInput();

	UFUNCTION(BlueprintCallable)
	void UnblockPrimaryAbilityInput();

	UFUNCTION(BlueprintPure)
	bool IsPrimaryAbilityInputBlocked() const;

	UFUNCTION(BlueprintCallable)
	void BlockSecondaryAbilityInput();

	UFUNCTION(BlueprintCallable)
	void UnblockSecondaryAbilityInput();

	UFUNCTION(BlueprintPure)
	bool IsSecondaryAbilityInputBlocked() const;

	UFUNCTION(BlueprintCallable)
	void CancelAbilitiesByTag(const FGameplayTagContainer& TagContainer);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag BaseGameplayTag;

	UPROPERTY(BlueprintAssignable)
	FOnGameplayTagCountChanged OnAnyGameplayTagCountChanged;

	UPROPERTY(BlueprintAssignable)
	FOnBaseGameplayTagCountChanged OnBaseGameplayTagCountChanged;

private:

	UFUNCTION()
	void HandleAnyGameplayTagCountChanged(const FGameplayTag Tag, int32 Count);

	UFUNCTION()
	void HandleBaseGameplayTagCountChanged(const FGameplayTag Tag, int32 Count);
};
