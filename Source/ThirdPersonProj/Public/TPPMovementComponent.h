// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPPMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UTPPMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	virtual FVector ConstrainInputAcceleration(const FVector& InputAcceleration) const;

	virtual FRotator GetDeltaRotation(float DeltaTime) const;

	virtual FVector ConsumeInputVector() override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bWantsToRun = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bWantsToWalk = false;

	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsGliding = false;

public:

	UFUNCTION(BlueprintCallable)
	void SetWantsToRun(bool bNewWantsToRun);

	UFUNCTION(BlueprintPure)
	bool DoesCharacterWantToRun() const { return bWantsToRun; }

	UFUNCTION(BlueprintCallable)
	void SetWantsToWalk(bool bNewWantsToWalk);

	UFUNCTION(BlueprintPure)
	bool DoesCharacterWantToWalk() const { return bWantsToWalk; }

	UFUNCTION(BlueprintCallable)
	void SetIsGliding(bool bNewIsGliding);

	UFUNCTION(BlueprintPure)
	bool IsGliding() const { return IsFalling() && bIsGliding; }
	
};
