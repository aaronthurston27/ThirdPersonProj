// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TPPAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHandleGameplayEffectApplied, UAbilitySystemComponent*, AbilitySystem, const FGameplayEffectSpec&, EffectSpec, FActiveGameplayEffectHandle, EffectHandle, const FGameplayTagContainer&, SourceTags);

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

	/** Base gameplay tag associated with this ability system. Used to determine elemental combinations */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag BaseGameplayTag;

	UPROPERTY(BlueprintAssignable)
	FHandleGameplayEffectApplied OnGameplayEffectApplied;

private:

	UFUNCTION()
	void OnGameplayEffectAppliedWrapper(UAbilitySystemComponent* ABS, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
};
