// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonProjCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TPPMovementComponent.h"
#include "TPPAbilitySystemComponent.h"
#include "TPPAbilitySetManager.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AThirdPersonProjCharacter

AThirdPersonProjCharacter::AThirdPersonProjCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	TPPAbilitySystemComponent = CreateDefaultSubobject<UTPPAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySetManager = CreateDefaultSubobject<UTPPAbilitySetManager>(TEXT("Ability Set Manager"));
}

void AThirdPersonProjCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (!bRootMotionMovementMode)
	{
		Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
	}
	else
	{
		Internal_AddMovementInput(WorldDirection * ScaleValue, bForce);
	}
}

void AThirdPersonProjCharacter::ConsumeRawInputVector()
{
	LastRawInputVector = RawInputVector;
	RawInputVector = FVector2D::ZeroVector;
}

void AThirdPersonProjCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (TPPAbilitySystemComponent && InputComponent)
	{
		const FTopLevelAssetPath InputEnumPath = FTopLevelAssetPath(TEXT("/Script/GameplayAbilities"), TEXT("EGameplayAbilityInputBinds"));
		const FGameplayAbilityInputBinds Binds(TEXT("Confirm"), TEXT("Cancel"), InputEnumPath, 0 ,1);
		TPPAbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, Binds);

		// Call this on begin play so that the controller pointer is set in the actor info struct.
		TPPAbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

UTPPMovementComponent* AThirdPersonProjCharacter::GetThirdPersonMovementComponent() const
{
	return Cast<UTPPMovementComponent>(GetCharacterMovement());
}

void AThirdPersonProjCharacter::OnJumped_Implementation()
{
	OnJumpedEvent.Broadcast();
}

FVector2D AThirdPersonProjCharacter::GetDesiredMovementVector_Implementation() const
{
	return GetLastRawInputVector();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AThirdPersonProjCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		// Third person proj - Removing these bindings so that the jump ability can handle this logic.
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonProjCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonProjCharacter::Look);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &AThirdPersonProjCharacter::OnWalkPressed);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AThirdPersonProjCharacter::OnRunPressed);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AThirdPersonProjCharacter::OnRunReleased);

		EnhancedInputComponent->BindAction(CyclePrimaryAbilityAction, ETriggerEvent::Started, this, &AThirdPersonProjCharacter::OnCyclePrimaryAbilityPressed);
		EnhancedInputComponent->BindAction(CycleSecondaryAbilityAction, ETriggerEvent::Started, this, &AThirdPersonProjCharacter::OnCycleSecondaryAbilityPressed);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AThirdPersonProjCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		RawInputVector = FVector2D(MovementVector.Y, MovementVector.X);
	}

}

void AThirdPersonProjCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThirdPersonProjCharacter::OnWalkPressed(const FInputActionValue& Value)
{
	UTPPMovementComponent* MoveComp = GetThirdPersonMovementComponent();
	if (MoveComp)
	{
		const bool bDoesCharacterWantToWalk = MoveComp->DoesCharacterWantToWalk();
		MoveComp->SetWantsToWalk(!bDoesCharacterWantToWalk);
	}
}

void AThirdPersonProjCharacter::OnWalkReleased(const FInputActionValue& Value)
{
	/*
	UTPPMovementComponent* MoveComp = GetThirdPersonMovementComponent();
	if (MoveComp)
	{
		MoveComp->SetWantsToWalk(false);
	}
	*/
}

void AThirdPersonProjCharacter::OnRunPressed(const FInputActionValue& Value)
{
	UTPPMovementComponent* MoveComp = GetThirdPersonMovementComponent();
	if (MoveComp)
	{
		MoveComp->SetWantsToRun(true);
		MoveComp->SetWantsToWalk(false);
	}
}

void AThirdPersonProjCharacter::OnRunReleased(const FInputActionValue& Value)
{
	UTPPMovementComponent* MoveComp = GetThirdPersonMovementComponent();
	if (MoveComp)
	{
		MoveComp->SetWantsToRun(false);
	}
}

void AThirdPersonProjCharacter::OnCyclePrimaryAbilityPressed(const FInputActionValue& Value)
{
	if (AbilitySetManager)
	{
		AbilitySetManager->SelectNextPrimaryAbility();
	}
}

void AThirdPersonProjCharacter::OnCycleSecondaryAbilityPressed(const FInputActionValue& Value)
{
	if (AbilitySetManager)
	{
		AbilitySetManager->SelectNextSecondaryAbility();
	}
}

