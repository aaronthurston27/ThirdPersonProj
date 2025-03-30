// Fill out your copyright notice in the Description page of Project Settings.


#include "TPPFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


TArray<FHitResult> UTPPFunctionLibrary::GetAllOverlapsWithinCone(const UObject* WorldContextObject, const FVector& ConeOrigin, const FRotator& ConeRotation, const float ConeLength, const float ConeWidthAngle, const float ConeHeightAngle, const TArray<AActor*>& IgnoreActors, bool bRequireLOS, bool bDrawDebug)
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

	if (bDrawDebug)
	{
		UKismetSystemLibrary::DrawDebugConeInDegrees(World, ConeOrigin, ConeRotation.Vector(), ConeLength, ConeWidthAngle / 2.0f, ConeHeightAngle / 2.0f, 12, FColor::Green, 14.0f, .4f);
		//DrawDebugSphere(World, SweepPosition, SphereCollision.GetSphereRadius(), 16.0f, FColor::Red, false, 14, 0, .3f);
	}
	World->SweepMultiByObjectType(OutHits, ConeOrigin, SweepPosition, ConeRotation.Quaternion(), FCollisionObjectQueryParams::AllDynamicObjects, SphereCollision, QueryParams);

	if (bRequireLOS)
	{
		QueryParams.bIgnoreBlocks = false;
	}

	for (const FHitResult& HitResult : OutHits)
	{
		const FVector OriginToHitLocation = HitResult.GetComponent()->GetComponentLocation() - ConeOrigin;
		if (bDrawDebug)
		{
			DrawDebugLine(World, ConeOrigin, HitResult.ImpactPoint, FColor::Yellow, false, 14, 0, .4f);
		}
		if (OriginToHitLocation.SizeSquared() >= LengthSqr)
		{
			continue;
		}

		FRotator Rotation = UKismetMathLibrary::MakeRotFromX(OriginToHitLocation) - ConeRotation;
		Rotation.Normalize();

		if (FMath::Abs(Rotation.Yaw) > (ConeWidthAngle / 2.0f)  || FMath::Abs(Rotation.Pitch) > (ConeHeightAngle / 2.0f))
		{
			continue;
		}

		if (bRequireLOS)
		{
			FHitResult LOSHitResult;
			World->LineTraceSingleByObjectType(LOSHitResult, ConeOrigin, HitResult.ImpactPoint, FCollisionObjectQueryParams::AllStaticObjects, QueryParams);
			if (LOSHitResult.bBlockingHit)
			{
				continue;
			}
		}

		OverlapsToReturn.Add(HitResult);
	}

	return OverlapsToReturn;
}

TArray<FHitResult> UTPPFunctionLibrary::GetOverlappingActorsInRadius(const UObject* WorldContextObject, const FVector& Origin, const float Radius, const TArray<AActor*>& IgnoreActors, bool bRequireLOS, bool bDrawDebug)
{
	TArray<FHitResult> OverlapsToReturn;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World || FMath::IsNearlyZero(Radius))
	{
		return OverlapsToReturn;
	}

	const FCollisionShape SphereCollision = FCollisionShape::MakeSphere(Radius);

	TArray<FHitResult> OutHits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(IgnoreActors);
	QueryParams.bFindInitialOverlaps = true;
	QueryParams.bIgnoreBlocks = true;

	if (bDrawDebug)
	{
		DrawDebugSphere(World, Origin, SphereCollision.GetSphereRadius(), 16.0f, FColor::Yellow, false, 14, 0, .3f);
	}
	World->SweepMultiByObjectType(OutHits, Origin, Origin, FQuat::Identity, FCollisionObjectQueryParams::AllDynamicObjects, SphereCollision, QueryParams);

	for (const FHitResult& HitResult : OutHits)
	{
		if (bRequireLOS)
		{
			FHitResult LOSHitResult;
			World->LineTraceSingleByObjectType(LOSHitResult, Origin, HitResult.ImpactPoint, FCollisionObjectQueryParams::AllStaticObjects, QueryParams);
			if (LOSHitResult.bBlockingHit)
			{
				continue;
			}
		}

		OverlapsToReturn.Add(HitResult);
	}

	return OverlapsToReturn;
}

bool UTPPFunctionLibrary::RaySphereIntersection(const FVector& RayDirection, const FVector& RayOrigin, const FVector& SphereCenter, const float Radius, FVector& Intersect1)
{
	const FVector L = RayOrigin - SphereCenter;
	float a = RayDirection | RayDirection;
	float b = 2 * (L | RayDirection);
	float c = (L | L) - (Radius * Radius);

	float t1, t2;
	const float discriminant = (b * b) - (4 * a * c);
	if (discriminant > 0)
	{
		t1 = (-b - sqrt(discriminant)) / (2 * a);
		t2 = (-b + sqrt(discriminant)) / (2 * a);
		t1 = FMath::Max(0, t1);
		t2 = FMath::Max(0, t2);

		Intersect1 = RayOrigin + (RayDirection * FMath::Min(t1, t2));
	}
	else if (discriminant == 0.0f)
	{
		t1 = -b / (2 * a);
		Intersect1 = RayOrigin + (RayDirection * t1);
	}
	else
	{
		return false;
	}

	return true;
}

float UTPPFunctionLibrary::ShortestDistanceToPlane(const FVector& Point, const FVector& PlaneNormal, const FVector& PointOnPlane)
{
	// Assume plane normal is normalized.
	const FVector L = Point - PointOnPlane;
	const float Point_PlaneDot = L | PlaneNormal;
	const FVector ProjectedPoint = Point_PlaneDot * PlaneNormal;
	return (ProjectedPoint - Point).Size();
}

FVector UTPPFunctionLibrary::ClosestPointOnLine(const FVector& Point, const FVector& LineStart, const FVector& LineEnd)
{
	const FVector Segment = LineEnd - LineStart;
	const FVector PointToStart = Point - LineStart;

	const float Point_SegmentDot = PointToStart | Segment;
	if (Point_SegmentDot <= 0)
	{
		return LineStart;
	}
	else if (Point_SegmentDot >= (Segment | Segment))
	{
		return LineEnd;
	}

	const FVector Projection = ((Segment | PointToStart) / (Segment | Segment)) * Segment;
	return LineStart + Projection;

}

bool UTPPFunctionLibrary::IsActorAimingAtBack(const FVector& ActorLocation, const FVector& ActorDirection, const FVector& TargetLocation, const FVector& TargetDirection)
{
	const float Target_ToActorDot = (TargetLocation - ActorLocation) | ActorDirection;
	if (Target_ToActorDot <= 0)
	{
		return false;
	}

	const float DirectionDot = TargetDirection | ActorDirection;
	return DirectionDot >= .75f;
}

bool UTPPFunctionLibrary::SphereAABBIntersection(const FVector& SphereCenter, const float SphereRadius, const FVector BoxCenter, const FVector BoxExtents)
{
	const FVector BoxMinPoints = BoxCenter - BoxExtents;
	const FVector BoxMaxPoints = BoxCenter + BoxExtents;

	const FVector ClosestPoint = FVector(FMath::Clamp(BoxMinPoints.X, SphereCenter.X, BoxMaxPoints.X), FMath::Clamp(BoxMinPoints.Y, SphereCenter.Y, BoxMaxPoints.Y), FMath::Clamp(BoxMinPoints.Z, SphereCenter.Z, BoxMaxPoints.Z));
	return (ClosestPoint - BoxCenter).SizeSquared() <= SphereRadius * SphereRadius;
}

void UTPPFunctionLibrary::ComputeSlopeOrientation(FVector& ForwardDirection, FVector& SlopeNormal, FVector& OutForward, FVector& OutUp)
{
	if (!ForwardDirection.IsNearlyZero() && !SlopeNormal.IsNearlyZero())
	{
		ForwardDirection = ForwardDirection.GetSafeNormal();
		SlopeNormal = SlopeNormal.GetSafeNormal();

		const FVector NewRight = ForwardDirection.GetSafeNormal() ^ SlopeNormal.GetSafeNormal();
		const FVector NewForward = SlopeNormal.GetSafeNormal() ^ NewRight.GetSafeNormal();

		OutForward = NewForward;
		OutUp = SlopeNormal;
	}
}

bool UTPPFunctionLibrary::RayPlaneIntersection(const FVector& RayDirection, const FVector& RayOrigin, const FVector& PointOnPlane, const FVector& PlaneNormal)
{
	float Normal_OriginDot = RayOrigin | PlaneNormal;
	float Normal_PlanePointDot = PointOnPlane | PlaneNormal;
	float Normal_DirectionDot = RayDirection | PlaneNormal;
	const float t = (Normal_PlanePointDot - Normal_OriginDot) / Normal_DirectionDot;
	return t >= 0;
}
