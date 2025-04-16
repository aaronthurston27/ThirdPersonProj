// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPMovementComponent.h"
#include "TPP_NativeGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

void UTPPMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(GetOwner());
	check(TPPCharacter);

	UAbilitySystemComponent* ASC = TPPCharacter->GetAbilitySystemComponent();
	check(ASC);

	ASC->RegisterGameplayTagEvent(TAG_Movement_BlockSprint, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UTPPMovementComponent::OnBlockSprintTagChanged);
}

FVector UTPPMovementComponent::ConstrainInputAcceleration(const FVector& InputAcceleration) const
{
	AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(GetOwner());
	if (TPPCharacter && TPPCharacter->IsUsingRootMotionMovement() && IsMovingOnGround())
	{
		return FVector::ZeroVector;
	}

	return Super::ConstrainInputAcceleration(InputAcceleration);
}

FRotator UTPPMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (bAllowPhysicsRotationDuringAnimRootMotion)
	{
		if (GetLastInputVector().IsNearlyZero())
		{
			return FRotator::ZeroRotator;
		}
	}

	return Super::GetDeltaRotation(DeltaTime);
}

FVector UTPPMovementComponent::ConsumeInputVector()
{
	AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(GetOwner());
	if (TPPCharacter)
	{
		TPPCharacter->ConsumeRawInputVector();
	}

	return Super::ConsumeInputVector();
}

void UTPPMovementComponent::SetWantsToRun(bool bNewWantsToRun)
{
	bWantsToRun = bNewWantsToRun;
	bWantsToRun &= CanCharacterRun();

	if (bWantsToRun)
	{
		SetWantsToWalk(false);
	}
}

bool UTPPMovementComponent::CanCharacterRun() const
{
	AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(GetOwner());
	check(TPPCharacter);
	if (TPPCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_Movement_BlockSprint))
	{
		return false;
	}
	if (TPPCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_Movement_ForceWalk))
	{
		return false;
	}

	return true;
}

void UTPPMovementComponent::SetWantsToWalk(bool bNewWantsToWalk)
{
	bWantsToWalk = bNewWantsToWalk;
	if (bWantsToWalk)
	{
		SetWantsToRun(false);
	}
}

bool UTPPMovementComponent::DoesCharacterWantToWalk() const
{
	AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(GetOwner());
	check(TPPCharacter);

	return bWantsToWalk || TPPCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_Movement_ForceWalk);
}

void UTPPMovementComponent::OnBlockSprintTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		bWantsToRun = false;
	}
}
