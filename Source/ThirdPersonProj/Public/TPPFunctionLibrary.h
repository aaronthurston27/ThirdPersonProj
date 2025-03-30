// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TPPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UTPPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static TArray<FHitResult> GetAllOverlapsWithinCone(const UObject* WorldContextObject, const FVector& ConeOrigin, const FRotator& ConeRotation, const float ConeLength, const float ConeWidthAngle, const float ConeHeightAngle, const TArray<AActor*>& IgnoreActors, bool bRequireLOS = true, bool bDrawDebug = false);
	
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static TArray<FHitResult> GetOverlappingActorsInRadius(const UObject* WorldContextObject, const FVector& Origin, const float Radius, const TArray<AActor*>& IgnoreActors, bool bRequireLOS = true, bool bDrawDebug = false);
	bool RaySphereIntersection(const FVector& RayDirection, const FVector& RayOrigin, const FVector& SphereCenter, const float Radius, FVector& Intersect1);
	float ShortestDistanceToPlane(const FVector& Point, const FVector& PlaneNormal, const FVector& PointOnPlane);
	FVector ClosestPointOnLine(const FVector& Point, const FVector& LineStart, const FVector& LineEnd);
	bool IsActorAimingAtBack(const FVector& ActorLocation, const FVector& ActorDirection, const FVector& TargetLocation, const FVector& TargetDirection);
	bool SphereAABBIntersection(const FVector& SphereCenter, const float SphereRadius, const FVector BoxCenter, const FVector BoxExtents);
	void ComputeSlopeOrientation(FVector& ForwardDirection, FVector& SlopeNormal, FVector& OutForward, FVector& OutUp);
	bool RayPlaneIntersection(const FVector& RayDirection, const FVector& RayOrigin, const FVector& PointOnPlane, const FVector& PlaneNormal);
};
