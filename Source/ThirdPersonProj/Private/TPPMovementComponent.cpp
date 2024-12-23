// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPMovementComponent.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

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
	if (bWantsToRun)
	{
		SetWantsToWalk(false);
	}
}

void UTPPMovementComponent::SetWantsToWalk(bool bNewWantsToWalk)
{
	bWantsToWalk = bNewWantsToWalk;
	if (bWantsToWalk)
	{
		SetWantsToRun(false);
	}
}

void UTPPMovementComponent::SetIsGliding(bool bNewIsGliding)
{
	bIsGliding = bNewIsGliding;
}
