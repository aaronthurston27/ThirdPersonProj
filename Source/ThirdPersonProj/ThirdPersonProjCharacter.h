// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AbilitySystemComponent.h"
#include "ThirdPersonProjCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UTPPMovementComponent;
class UTPPAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AThirdPersonProjCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

protected:

	UPROPERTY(VisibleAnywhere, Transient, BlueprintReadOnly)
	UTPPAbilitySystemComponent* TPPAbilitySystemComponent;

public:

	AThirdPersonProjCharacter();

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;
	;
	void ConsumeRawInputVector();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void OnWalkPressed(const FInputActionValue& Value);

	void OnWalkReleased(const FInputActionValue& Value);

	void OnRunPressed(const FInputActionValue& Value);

	void OnRunReleased(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	UTPPAbilitySystemComponent* GetAbilitySystemComponent() const { return TPPAbilitySystemComponent; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRootMotionMovementMode = true;

	UPROPERTY(Transient)
	FVector2D LastRawInputVector = FVector2D::ZeroVector;

	UPROPERTY(Transient)
	FVector2D RawInputVector = FVector2D::ZeroVector;

public:

	UFUNCTION(BlueprintPure)
	FVector2D GetLastRawInputVector() const { return LastRawInputVector; }

	UFUNCTION(BlueprintPure)
	bool IsUsingRootMotionMovement() const { return bRootMotionMovementMode; }

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	FVector2D GetDesiredMovementVector() const;

	virtual FVector2D GetDesiredMovementVector_Implementation() const;

	UFUNCTION(BlueprintPure)
	UTPPMovementComponent* GetThirdPersonMovementComponent() const;
};
