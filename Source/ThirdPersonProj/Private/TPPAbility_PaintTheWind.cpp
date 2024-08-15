// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbility_PaintTheWind.h"
#include "AbilityActors/AbilityActor_WindPath.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"
#include "Kismet/GameplayStatics.h"

void UTPPAbility_PaintTheWind::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Destroy the old wind path.
	if (WindPathActor)
	{
		WindPathActor->Destroy();
	}
}

void UTPPAbility_PaintTheWind::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentPathLocation = FVector::ZeroVector;
	CurrentPathDirection = FVector::ZeroVector;
	OriginalPathDirection = FVector::ZeroVector;
	PathCenterPoint = FVector::ZeroVector;
	bPathCenterPointReached = false;
}

void UTPPAbility_PaintTheWind::UpdateWindPath(float DeltaTime)
{
	check(WindPathActor);
	if (!WindPathActor)
	{
		return;
	}

	if (bPathCenterPointReached)
	{
		AdjustWindDirection(DeltaTime);
	}

	const FVector TargetPathPoint = CurrentPathLocation + (CurrentPathDirection * PathDrawingSpeed * DeltaTime);
	const float LastSplinePoint = WindPathActor->PathSpline->GetNumberOfSplinePoints() - 1;
	const FVector PreviousPathPoint = WindPathActor->PathSpline->GetLocationAtSplinePoint(LastSplinePoint, ESplineCoordinateSpace::World);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, PreviousPathPoint, TargetPathPoint, FCollisionObjectQueryParams::AllStaticObjects);

	const FVector NewPoint = HitResult.bBlockingHit ? HitResult.Location : HitResult.TraceEnd;
	if (!HitResult.bBlockingHit || HitResult.Distance > 3.0f)
	{
		WindPathActor->AddPointToSplinePath(NewPoint, CurrentPathDirection);

		CurrentPathLocation = NewPoint;
		const FVector PointToCenter = NewPoint - PathCenterPoint;
		if (!bPathCenterPointReached)
		{
			const float PointCenter_DirectionDot = PointToCenter | CurrentPathDirection;
			if (PointCenter_DirectionDot > 0.0f)
			{
				bPathCenterPointReached = true;
			}
		}
		
		if (bPathCenterPointReached)
		{
			const float DistanceFromCenter = PointToCenter.Size();
			if (DistanceFromCenter >= MaxLengthFromCenterPoint)
			{
				OnWindPathCompleted();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Update failed. Size: %f"), (NewPoint - PreviousPathPoint).Size());
		OnWindPathCompleted();
	}
}

void UTPPAbility_PaintTheWind::CreateWindPath(const FVector& WindPathStartingPoint, const FVector& WindPathCenterPoint, const FRotator& Rotation)
{
	FTransform SpawnTransform(Rotation, WindPathStartingPoint);
	FActorSpawnParameters SpawnParams;
	WindPathActor = GetWorld()->SpawnActor<AAbilityActor_WindPath>(WindPathClass.Get(), SpawnTransform);

	ensure(WindPathActor);
	if (!WindPathActor)
	{
		return;
	}

	CurrentPathLocation = WindPathStartingPoint;
	PathCenterPoint = WindPathCenterPoint;
	CurrentPathDirection = OriginalPathDirection = Rotation.Vector();
	bPathCenterPointReached = false;

	WindPathActor->PathSpline->ClearSplinePoints();
	WindPathActor->AddPointToSplinePath(CurrentPathLocation, CurrentPathDirection);
		
}

void UTPPAbility_PaintTheWind::SetWantsToCurvePath(bool bNewWantsToCurvePath)
{
	bWantsToCurveWindPath = bNewWantsToCurvePath;
}

void UTPPAbility_PaintTheWind::AdjustWindDirection(float DeltaTime)
{
	const FRotator TargetRotation = GetTargetWindDirection().ToOrientationRotator();

	FRotator DeltaToApply = TargetRotation - CurrentPathDirection.Rotation();
	DeltaToApply.Normalize();
	DeltaToApply.Pitch = FMath::Clamp(DeltaToApply.Pitch, -DeltaTime * PathRotationSpeed.Pitch, DeltaTime * PathRotationSpeed.Pitch);
	DeltaToApply.Yaw = FMath::Clamp(DeltaToApply.Yaw, -DeltaTime * PathRotationSpeed.Yaw, DeltaTime * PathRotationSpeed.Yaw);

	FRotator DirectionRot = CurrentPathDirection.Rotation() + DeltaToApply;
	DirectionRot.Normalize();

	CurrentPathDirection = DirectionRot.Vector();
}

FVector UTPPAbility_PaintTheWind::GetTargetWindDirection() const
{
	if (bPathCenterPointReached)
	{
		const FVector2D RawInputVector = CachedCharacterOwner->GetLastRawInputVector();
		if (!RawInputVector.IsNearlyZero())
		{
			const FVector CenterToOwner_Vec = (PathCenterPoint - CachedCharacterOwner->GetActorLocation()).GetSafeNormal2D();
			const float WindDirection_PathToCenterDot = FMath::RoundToFloat(OriginalPathDirection | CenterToOwner_Vec);
			FVector LocalSpaceVector_ToRotate = FVector(0.0f, RawInputVector.Y, RawInputVector.X).GetSafeNormal();
			if (LocalSpaceVector_ToRotate.IsNearlyZero())
			{
				LocalSpaceVector_ToRotate = FVector::ForwardVector;
			}

			// If wind path is travelling parallel to the player's look vector, horizontal (y axis) key input should curve the path to the players right or left.
			// Else, if the wind path is travelling perpindicular, curve left/right relative to the wind path.
			if (FMath::Abs(WindDirection_PathToCenterDot) > .8f)
			{
				if (FMath::Abs(LocalSpaceVector_ToRotate.Z) == 1.0f)
				{
					// If only pitching up, don't move past the max pitch. Don't move at 90 degrees to avoid gimbal lock.
					LocalSpaceVector_ToRotate = FVector(FMath::Sign(WindDirection_PathToCenterDot), 0.0f, LocalSpaceVector_ToRotate.Z * FMath::Asin(FMath::DegreesToRadians(MaxPathPitch))).GetSafeNormal();
				}
				return CenterToOwner_Vec.Rotation().RotateVector(LocalSpaceVector_ToRotate);
			}

			if (FMath::Abs(LocalSpaceVector_ToRotate.Z) == 1.0f)
			{
				// If only pitching up, don't move past the max pitch. Don't move at 90 degrees to avoid gimbal lock.
				LocalSpaceVector_ToRotate = FVector(1.0f, 0.0f, LocalSpaceVector_ToRotate.Z * FMath::Asin(FMath::DegreesToRadians(MaxPathPitch))).GetSafeNormal();
			}

			return OriginalPathDirection.Rotation().RotateVector(LocalSpaceVector_ToRotate);
		}
	}

	return CurrentPathDirection;
}

void UTPPAbility_PaintTheWind::OnWindPathActorCreated_Implementation()
{
}

void UTPPAbility_PaintTheWind::OnWindPathCompleted_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Path completed"));
	CurrentPathDirection = FVector::ZeroVector;
	WindPathActor->OnWindPathCompleted();
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

FGameplayAbilityTargetData_WindPaint UTPPAbility_PaintTheWind::GetTargetDataFromHandle(const FGameplayAbilityTargetDataHandle& Handle) const
{
	if (!Handle.Data.IsEmpty())
	{
		FGameplayAbilityTargetData_WindPaint* ReturnData;
		ReturnData = static_cast<FGameplayAbilityTargetData_WindPaint*>(Handle.Data[0].Get());
		return *ReturnData;
	}

	return FGameplayAbilityTargetData_WindPaint();
}
