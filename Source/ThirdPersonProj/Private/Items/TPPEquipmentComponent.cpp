// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TPPEquipmentComponent.h"
#include "Items/TPPEquippableItem.h"

// Sets default values for this component's properties
UTPPEquipmentComponent::UTPPEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTPPEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UTPPEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UTPPEquipmentComponent::PickupItem(ATPPEquippableItem* ItemToPickup)
{
	if (!ensureAlways(ItemToPickup))
	{
		return false;
	}

	if (CurrentEquippedItem)
	{
		DropCurrentItem();
	}

	if (CurrentEquippedItem)
	{
		return false;
	}

	if (ItemToPickup->CanBePickedUpBy(this))
	{
		CurrentEquippedItem = ItemToPickup;
		CurrentEquippedItem->OnPickedUpBy(this);
		CurrentEquippedItem->TryEquip();

		return true;
	}

	return false;
}

ATPPEquippableItem* UTPPEquipmentComponent::DropCurrentItem()
{
	if (CurrentEquippedItem)
	{
		ATPPEquippableItem* OldItem = CurrentEquippedItem;
		OldItem->DropItem();
		return OldItem;
	}

	return nullptr;
}

