// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActors/AbilityTargetActor_WindPaint.h"
#include "TPPAbility_PaintTheWind.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"

void AAbilityTargetActor_WindPaint::StartTargeting(UGameplayAbility* InAbility)
{
	Super::StartTargeting(InAbility);

	const AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(SourceActor);
	if (TPPCharacter)
	{
		TPPCharacter->InputComponent->BindAxis(FName(TEXT("AbilityScroll")), this, &AAbilityTargetActor_WindPaint::OnAbilityScrollAxisValueChanged);
	}

	// Cache max range so that we can't exceed it when changing the trace range through input.
	CachedMaxRange = MaxRange;
}

void AAbilityTargetActor_WindPaint::Tick(float DeltaSeconds)
{
	const AThirdPersonProjCharacter* TPPCharacter = Cast<AThirdPersonProjCharacter>(SourceActor);
	if (TPPCharacter)
	{
		const FVector2D RawInput = TPPCharacter->GetLastRawInputVector();
		if (!RawInput.IsNearlyZero())
		{
			WindDirection_LocalSpace = FVector(RawInput.X, RawInput.Y, 0.0f);
		}
	}

	Super::Tick(DeltaSeconds);

	const FVector NewActorRotation = SourceActor->GetTransform().TransformVector(WindDirection_LocalSpace);
	SetActorRotation(NewActorRotation.ToOrientationQuat());
}

void AAbilityTargetActor_WindPaint::OnAbilityScrollAxisValueChanged(float AxisValue)
{
	if (!FMath::IsNearlyZero(AxisValue))
	{
		MaxRange += AxisValue * TraceDistanceModulationValue;
		MaxRange = FMath::Clamp(MaxRange, MinimumTraceDistance, CachedMaxRange);
	}
}

FHitResult AAbilityTargetActor_WindPaint::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AGameplayAbilityTargetActor_WindPaint), bTraceComplex);
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(InSourceActor);

	FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation();
	FVector TraceEnd;
	AimWithPlayerController(InSourceActor, Params, TraceStart, TraceEnd);

	FHitResult ReturnHitResult;
	//Use a line trace initially to see where the player is actually pointing
	LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);
	//Default to end of trace line if we don't hit anything.
	if (!ReturnHitResult.bBlockingHit)
	{
		ReturnHitResult.Location = TraceEnd;
	}

	if (!WindDirection_LocalSpace.Equals(FVector::ZeroVector))
	{
		const FVector RotatedTraceVector = StartLocation.GetTargetingTransform().TransformVector(WindDirection_LocalSpace);
		const FVector DesiredWindStartLocation = ReturnHitResult.Location + (ReturnHitResult.ImpactNormal * 3.0f) - (RotatedTraceVector * MaxRange * .80f);

		FHitResult IntermediateHitResult;
		LineTraceWithFilter(IntermediateHitResult, InSourceActor->GetWorld(), Filter, TraceStart, DesiredWindStartLocation, TraceProfile.Name, Params);
		if (!IntermediateHitResult.bBlockingHit)
		{
			IntermediateHitResult.Location = IntermediateHitResult.TraceEnd;
		}
		// DrawDebugSphere(InSourceActor->GetWorld(), IntermediateHitResult.Location, 60.0f, 6, FColor::Yellow, false, .8f, 0, .3f);

		if (IntermediateHitResult.Time < .6f)
		{
			bWasLastTraceValid = false;
			DrawDebugSphere(InSourceActor->GetWorld(), IntermediateHitResult.Location, 60.0f, 6, FColor::Red, false, .8f, 0, .3f);
			return ReturnHitResult;
		}

		TraceStart = IntermediateHitResult.Location + (IntermediateHitResult.ImpactNormal * 3.0f);
		LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, ReturnHitResult.Location, TraceProfile.Name, Params);

	}

	if (ReturnHitResult.Distance < MinimumTraceDistance)
	{
		bWasLastTraceValid = false;
	}

	DrawDebugSphere(InSourceActor->GetWorld(), ReturnHitResult.TraceStart, 60.0f, 6, FColor::Green, false, .8f, 0, .3f);
	
	bWasLastTraceValid = true;
	return ReturnHitResult;
}

bool AAbilityTargetActor_WindPaint::IsConfirmTargetingAllowed()
{
	return bWasLastTraceValid;
}

void AAbilityTargetActor_WindPaint::SetWindDirectionLocalSpace(const FVector& WindDirectionLocalSpace)
{
	WindDirection_LocalSpace = WindDirectionLocalSpace;
}

void AAbilityTargetActor_WindPaint::ConfirmTargetingAndContinue()
{
	const FHitResult ConfirmResult = PerformTrace(SourceActor);
	
	FGameplayAbilityTargetDataHandle ReturnHandle;

	FGameplayAbilityTargetData_WindPaint* ReturnData = new FGameplayAbilityTargetData_WindPaint();
	ReturnData->StartingPosition = ConfirmResult.TraceStart;
	ReturnData->WindTravelDirection = (ConfirmResult.Location - ConfirmResult.TraceStart).GetSafeNormal();
	ReturnData->PathCenterPoint = ConfirmResult.bBlockingHit ? ConfirmResult.TraceEnd : ConfirmResult.Location;

	ReturnHandle.Add(ReturnData);
	TargetDataReadyDelegate.Broadcast(ReturnHandle);
}
