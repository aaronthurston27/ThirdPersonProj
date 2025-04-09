// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AT_Tick.h"

UAT_Tick::UAT_Tick(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bTickingTask = true;
}

UAT_Tick* UAT_Tick::AbilityTask_OnTick(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	UAT_Tick* MyObj = NewAbilityTask<UAT_Tick>(OwningAbility);
	return MyObj;
}

void UAT_Tick::Activate()
{
	Super::Activate();
}

void UAT_Tick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnTick.Broadcast(DeltaTime);
	}
}