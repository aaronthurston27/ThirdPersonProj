// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPPAbility.h"
#include "TPPAbility_Tornado.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UTPPAbility_Tornado : public UTPPAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MinimumTornadoSpawnDistance = 50.0f;

public:

	FVector FindTornadoSpawnLocation() const;
	
	bool IsValidTornadoSpawnLocation(const FVector& SpawnLocation) const;
};
