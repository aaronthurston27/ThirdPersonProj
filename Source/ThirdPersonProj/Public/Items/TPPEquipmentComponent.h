// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPPEquipmentComponent.generated.h"

class ATPPEquippableItem;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONPROJ_API UTPPEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPPEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintCallable)
	bool PickupItem(ATPPEquippableItem* ItemToPickup);

	UFUNCTION(BlueprintCallable)
	ATPPEquippableItem* DropCurrentItem();

protected:

	UPROPERTY(Transient, BlueprintReadOnly)
	ATPPEquippableItem* CurrentEquippedItem;
		
};
