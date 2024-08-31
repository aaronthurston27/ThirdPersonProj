// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/TPPAbility_Tornado.h"

FVector UTPPAbility_Tornado::FindTornadoSpawnLocation() const
{
	return FVector();
}

bool UTPPAbility_Tornado::IsValidTornadoSpawnLocation(const FVector& SpawnLocation) const
{
	return (GetOwningActorFromActorInfo()->GetActorLocation() - SpawnLocation).Size() >= MinimumTornadoSpawnDistance;
}
