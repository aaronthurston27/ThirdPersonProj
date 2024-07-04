// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TPPAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
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


};
