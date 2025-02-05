// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "TPPEquippableItem.generated.h"

class UTPPAbility;
class UTPPAbilitySet;
class UTPPEquipmentComponent;
class UAbilitySystemComponent;
class AThirdPersonProjCharacter;

UENUM(BlueprintType)
enum class EEquippableState : uint8
{
	Invalid,
	Dropped,
	Unequipped,
	Unequipping,
	Equipping,
	Equipped
};

USTRUCT(BlueprintType)
struct FEquipItemData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FName AttachBoneName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FTransform EquippedAttachOffset = FTransform::Identity;

	UPROPERTY(EditDefaultsOnly)
	FName HolsterBoneName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* UnequipMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimSequence* IdleAnimation = nullptr;
};

UCLASS(Abstract, BlueprintType, Blueprintable)
class THIRDPERSONPROJ_API ATPPEquippableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPPEquippableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TWeakObjectPtr<UTPPAbilitySet> AbilitiesToGrant;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FEquipItemData EquipData;

	UPROPERTY(BlueprintReadOnly, Transient)
	TWeakObjectPtr<UTPPEquipmentComponent> OwnerEquipmentComp;

	UPROPERTY(BlueprintReadOnly, Transient)
	EEquippableState EquipState = EEquippableState::Invalid;

public:

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanBePickedUpBy(UTPPEquipmentComponent* Comp) const;

	bool CanBePickedUpBy_Implementation(UTPPEquipmentComponent* Comp) const;

	void OnPickedUpBy(UTPPEquipmentComponent* Comp);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanEquip() const;
	bool CanEquip_Implementation() const;

	UFUNCTION(BlueprintCallable)
	void TryEquip(bool bInstantEquip = false);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanUnequip() const ;
	bool CanUnequip_Implementation() const;
	void TryUnequip(bool bInstantUnequip = false);

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION(BlueprintPure)
	AThirdPersonProjCharacter* GetOwnerCharacter() const;

	UFUNCTION(BlueprintPure)
	UAbilitySystemComponent* GetOwnerAbilitySystem() const;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnPickedUpBy(UTPPEquipmentComponent* Comp);

	void OnDropped();

	void BeginEquip();

	void BeginUnequip();

	void SetEquipState(EEquippableState NewState);

	void OnEquipStateChanged(EEquippableState PrevState, EEquippableState NewState);

	void DetachMesh();

	void AttachToOwner(const FName& SocketName);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnEquipStateChanged(EEquippableState PrevState, EEquippableState NewState);

	void GrantEquippableAbilities();

	void RemoveEquippableAbilities();

protected:

	UPROPERTY(Transient)
	bool bHasGrantedAbilties = false;

	UPROPERTY(Transient)
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;

};
