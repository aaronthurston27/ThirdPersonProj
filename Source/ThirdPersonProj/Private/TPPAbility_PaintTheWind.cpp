// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPAbility_PaintTheWind.h"
#include "AbilityActors/AbilityActor_WindPath.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"
#include "Kismet/GameplayStatics.h"

void UTPPAbility_PaintTheWind::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTPPAbility_PaintTheWind::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentPathLocation = FVector::ZeroVector;
	CurrentPathDirection = FVector::ZeroVector;
	OriginalPathDirection = FVector::ZeroVector;
	PathCenterPoint = FVector::ZeroVector;
	WindPathActor = nullptr;
	bPathCenterPointReached = false;
}

void UTPPAbility_PaintTheWind::UpdateWindPath(float DeltaTime)
{
	check(WindPathActor);
	if (!WindPathActor)
	{
		return;
	}

	AdjustWindDirection(DeltaTime);

	const FVector TargetPathPoint = CurrentPathLocation + (CurrentPathDirection * PathDrawingSpeed * DeltaTime);
	const float LastSplinePoint = WindPathActor->PathSpline->GetSplineLength();
	const FVector PreviousPathPoint = WindPathActor->PathSpline->GetLocationAtSplinePoint(LastSplinePoint, ESplineCoordinateSpace::World);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, PreviousPathPoint, TargetPathPoint, FCollisionObjectQueryParams::AllStaticObjects);

	const FVector NewPoint = HitResult.bBlockingHit ? HitResult.Location : HitResult.TraceEnd;
	const float NewSplineIndex = LastSplinePoint + 1;
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
	WindPathActor->PathSpline->AddSplinePoint(CurrentPathLocation, ESplineCoordinateSpace::World);
		
}

void UTPPAbility_PaintTheWind::SetWantsToCurvePath(bool bNewWantsToCurvePath)
{
	bWantsToCurveWindPath = bNewWantsToCurvePath;
}

void UTPPAbility_PaintTheWind::AdjustWindDirection(float DeltaTime)
{
	if (bWantsToCurveWindPath && bPathCenterPointReached)
	{
		FVector PlayerViewPoint;
		FRotator Rot;
		CachedCharacterOwner->GetController()->GetPlayerViewPoint(PlayerViewPoint, Rot);

		const FVector PlayerAimVector = Rot.Vector();
		const FVector CenterToPlayerAimVector = (PathCenterPoint - PlayerViewPoint);
		const FRotator RotationDelta = (PlayerAimVector.Rotation() - CenterToPlayerAimVector.Rotation()) * LookInputPathControlScalar;
		//UE_LOG(LogTemp, Warning, TEXT("Rotation delta: %s, Scalar: %s"), *RotationDelta.ToString(), *(RotationDelta * LookInputPathControlScalar).ToString());

		const FRotator OriginalRotation = OriginalPathDirection.Rotation();
		FRotator TargetRotation = OriginalRotation + RotationDelta;

		TargetRotation.Pitch = FMath::Clamp(TargetRotation.Pitch, OriginalRotation.Pitch - 90.0f, OriginalRotation.Pitch + 90.0f);
		TargetRotation.Yaw = FMath::Clamp(TargetRotation.Yaw, OriginalRotation.Yaw - 90.0f, OriginalRotation.Yaw + 90.0f);
		//UE_LOG(LogTemp, Warning, TEXT("Target Rotation: %s, Original: %s"), *TargetRotation.ToString(), *(OriginalRotation).ToString());

		FRotator DeltaToApply = TargetRotation - CurrentPathDirection.Rotation();
		DeltaToApply.Pitch = FMath::Clamp(DeltaToApply.Pitch, -DeltaTime * PathRotationSpeed.Pitch, DeltaTime * PathRotationSpeed.Pitch);
		DeltaToApply.Yaw = FMath::Clamp(DeltaToApply.Yaw, -DeltaTime * PathRotationSpeed.Yaw, DeltaTime * PathRotationSpeed.Yaw);

		FRotator DirectionRot = CurrentPathDirection.Rotation() + DeltaToApply;
		DirectionRot.Normalize();

		CurrentPathDirection = DirectionRot.Vector();
		//UE_LOG(LogTemp, Warning, TEXT("DeltaToApply: %s, Direction: %s"), *DeltaToApply.ToString(), *CurrentPathDirection.ToString());
	}
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
