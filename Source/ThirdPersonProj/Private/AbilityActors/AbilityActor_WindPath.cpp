// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActors/AbilityActor_WindPath.h"

// Sets default values
AAbilityActor_WindPath::AAbilityActor_WindPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	SetRootComponent(RootSceneComponent);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Path Spline"));
	PathSpline->SetupAttachment(RootSceneComponent);
}

// Called when the game starts or when spawned
void AAbilityActor_WindPath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbilityActor_WindPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityActor_WindPath::OnWindPathCompleted_Implementation()
{
	GetWorldTimerManager().SetTimer(WindPathTimerHandle, this, &AAbilityActor_WindPath::OnWindPathDurationExpired, WindPathDuration, false);
}

void AAbilityActor_WindPath::OnWindPathDurationExpired_Implementation()
{
	SetLifeSpan(WindPathExpiredDuration);
}

