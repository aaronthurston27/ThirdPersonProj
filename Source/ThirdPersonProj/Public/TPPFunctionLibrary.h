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
	static TArray<FHitResult> GetAllOverlapsWithinCone(const UObject* WorldContextObject, const FVector& ConeOrigin, const FRotator& ConeRotation, const float ConeLength, const float ConeWidthAngle, const float ConeHeightAngle, const TArray<AActor*>& IgnoreActors, bool bRequireLOS = true);
	
};
