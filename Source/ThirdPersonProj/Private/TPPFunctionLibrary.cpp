// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


TArray<FHitResult> UTPPFunctionLibrary::GetAllOverlapsWithinCone(const UObject* WorldContextObject, const FVector& ConeOrigin, const FRotator& ConeRotation, const float ConeLength, const float ConeWidthAngle, const float ConeHeightAngle, const TArray<AActor*>& IgnoreActors, bool bRequireLOS)
{
	TArray<FHitResult> OverlapsToReturn;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World || ConeWidthAngle <= 0 || ConeHeightAngle <= 0)
	{
		return OverlapsToReturn;
	}

	const FVector ConeDirection = ConeRotation.Vector();
	const float LengthSqr = ConeLength * ConeLength;
	const float RadiusFromHeightAngle = ConeLength * FMath::Tan(FMath::DegreesToRadians(ConeHeightAngle));
	const float RadiusFromWidthAngle = ConeLength * FMath::Tan(FMath::DegreesToRadians(ConeWidthAngle));

	const FCollisionShape SphereCollision = FCollisionShape::MakeSphere(FMath::Max(RadiusFromWidthAngle, RadiusFromHeightAngle));
	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	QueryParams.bFindInitialOverlaps = true;
	QueryParams.bIgnoreBlocks = true;
	const FVector SweepPosition = ConeOrigin + (ConeDirection * ConeLength);

	UKismetSystemLibrary::DrawDebugConeInDegrees(World, ConeOrigin, ConeRotation.Vector(), ConeLength, ConeWidthAngle, ConeHeightAngle, 12, FColor::Green, 14.0f, .4f);
	//DrawDebugSphere(World, SweepPosition, SphereCollision.GetSphereRadius(), 16.0f, FColor::Red, false, 14, 0, .3f);
	World->SweepMultiByObjectType(OutHits, ConeOrigin, SweepPosition, ConeRotation.Quaternion(), FCollisionObjectQueryParams::AllDynamicObjects, SphereCollision, QueryParams);

	if (bRequireLOS)
	{
		TArray<AActor*> OverlappedActors;
		// Get all overlapped actors for LOS check. Maybe there's a better way to do this?
		for (const FHitResult& HitResult : OutHits)
		{
			OverlappedActors.Add(HitResult.GetActor());
		}

		QueryParams.bIgnoreBlocks = false;
		QueryParams.AddIgnoredActors(OverlappedActors);
	}

	for (const FHitResult& HitResult : OutHits)
	{
		const FVector OriginToHitLocation = HitResult.GetActor()->GetActorLocation() - ConeOrigin;
		DrawDebugLine(World, ConeOrigin, HitResult.ImpactPoint, FColor::Yellow, false, 14, 0, .4f);
		if (OriginToHitLocation.SizeSquared() >= LengthSqr)
		{
			continue;
		}

		FRotator Rotation = UKismetMathLibrary::MakeRotFromX(OriginToHitLocation) - ConeRotation;
		Rotation.Normalize();

		if (FMath::Abs(Rotation.Yaw) > ConeWidthAngle  || FMath::Abs(Rotation.Pitch) > ConeHeightAngle)
		{
			continue;
		}

		if (bRequireLOS)
		{
			FHitResult LOSHitResult;
			World->LineTraceSingleByChannel(LOSHitResult, ConeOrigin, HitResult.ImpactPoint, ECollisionChannel::ECC_WorldStatic, QueryParams, FCollisionResponseParams::DefaultResponseParam);
			if (LOSHitResult.bBlockingHit)
			{
				continue;
			}
		}

		OverlapsToReturn.Add(HitResult);
	}

	return OverlapsToReturn;
}