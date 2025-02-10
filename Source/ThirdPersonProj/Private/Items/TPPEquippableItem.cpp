// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/TPPEquippableItem.h"
#include "AbilitySystemInterface.h"
#include "Abilities/TPPEquipmentAbility.h"
#include "TPPAbilitySystemComponent.h"
#include "ThirdPersonProj/ThirdPersonProjCharacter.h"
#include "Items/TPPEquipmentComponent.h"

// Sets default values
ATPPEquippableItem::ATPPEquippableItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("Skeletal Mesh")));
	SetRootComponent(SkeletalMesh);
	if (SkeletalMesh)
	{
		SkeletalMesh->AlwaysLoadOnClient = true;
		SkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		SkeletalMesh->bCastDynamicShadow = true;
		SkeletalMesh->bAffectDynamicIndirectLighting = true;
		SkeletalMesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void ATPPEquippableItem::BeginPlay()
{
	Super::BeginPlay();

	SetEquipState(EEquippableState::Dropped);
}

// Called every frame
void ATPPEquippableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ATPPEquippableItem::CanBePickedUpBy_Implementation(UTPPEquipmentComponent* Comp) const
{
	if (OwnerEquipmentComp.IsValid())
	{
		return false;
	}

	return true;
}

void ATPPEquippableItem::OnPickedUpBy(UTPPEquipmentComponent* Comp)
{
	check(Comp != nullptr);
	check(OwnerEquipmentComp.IsExplicitlyNull());
	OwnerEquipmentComp = Comp;

	SetOwner(Comp->GetOwner());
	SetActorHiddenInGame(true);

	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	SetEquipState(EEquippableState::Unequipped);

	BP_OnPickedUpBy(Comp);
}

void ATPPEquippableItem::OnDropped()
{
	check(Owner == nullptr);
	check(OwnerEquipmentComp.IsExplicitlyNull());

	DetachMesh();

	if (Owner)
	{
		SetOwner(nullptr);
	}

	SkeletalMesh->SetSimulatePhysics(true);
	static const FName DroppedEquipmentCollisionProfile = FName(TEXT("Equipment"));
	SkeletalMesh->SetCollisionProfileName(DroppedEquipmentCollisionProfile);

	SetActorHiddenInGame(false);
}

bool ATPPEquippableItem::CanEquip_Implementation() const
{
	if (!ensureAlways(OwnerEquipmentComp.IsValid()))
	{
		return false;
	}

	return EquipState == EEquippableState::Unequipped || EquipState == EEquippableState::Unequipping;
}

void ATPPEquippableItem::TryEquip(bool bInstantEquip)
{
	if (CanEquip())
	{
		BeginEquip();
	}
}

bool ATPPEquippableItem::CanUnequip_Implementation() const
{
	return EquipState >= EEquippableState::Equipping;
}

void ATPPEquippableItem::TryUnequip(bool bInstantUnequip)
{
	if (CanUnequip())
	{
		if (bInstantUnequip)
		{
			SetEquipState(EEquippableState::Unequipped);
		}
		else
		{
			BeginUnequip();
		}
	}
}

void ATPPEquippableItem::DropItem()
{
	if (!CanUnequip())
	{
		return;
	}

	TryUnequip(true);
	SetOwner(nullptr);
	OwnerEquipmentComp = nullptr;
}

AThirdPersonProjCharacter* ATPPEquippableItem::GetOwnerCharacter() const
{
	if (!OwnerEquipmentComp.IsValid())
	{
		return nullptr;
	}

	return Cast<AThirdPersonProjCharacter>(OwnerEquipmentComp.Get()->GetOwner());
}

UAbilitySystemComponent* ATPPEquippableItem::GetOwnerAbilitySystem() const
{
	const AThirdPersonProjCharacter* TPPCharacter = GetOwnerCharacter();
	return TPPCharacter ? TPPCharacter->GetAbilitySystemComponent() : nullptr;
}

EEquippableState ATPPEquippableItem::GetEquippableState() const
{
	return EquipState;
}

void ATPPEquippableItem::BeginEquip()
{
	SetEquipState(EEquippableState::Equipped);
}

void ATPPEquippableItem::BeginUnequip()
{
	SetEquipState(EEquippableState::Unequipped);
}

void ATPPEquippableItem::SetEquipState(EEquippableState NewState)
{
	if (EquipState != NewState)
	{
		const EEquippableState PrevState = EquipState;
		EquipState = NewState;
		OnEquipStateChanged(PrevState, NewState);
	}
}

void ATPPEquippableItem::OnEquipStateChanged(EEquippableState PrevState, EEquippableState NewState)
{
	check(EquipState == NewState);

	switch (NewState)
	{
		case EEquippableState::Dropped:
			
			OnDropped();
			break;
		case EEquippableState::Unequipped:

			if (bHasGrantedAbilties)
			{
				RemoveEquippableAbilities();
			}

			if (!EquipData.HolsterBoneName.IsNone())
			{
				AttachToOwner(EquipData.HolsterBoneName);
				SetActorHiddenInGame(false);
			}
			else
			{
				SetActorHiddenInGame(true);
			}

			break;
		case EEquippableState::Unequipping:

			RemoveEquippableAbilities();
			break;

		case EEquippableState::Equipping:

			if (!bHasGrantedAbilties)
			{
				GrantEquippableAbilities();
			}

			check(!EquipData.AttachBoneName.IsNone());
			SetActorHiddenInGame(false);
			AttachToOwner(EquipData.AttachBoneName);
			break;

		case EEquippableState::Equipped:
			if (!bHasGrantedAbilties)
			{
				GrantEquippableAbilities();
			}

			check(!EquipData.AttachBoneName.IsNone());
			SetActorHiddenInGame(false);
			AttachToOwner(EquipData.AttachBoneName);
			break;
	}

	BP_OnEquipStateChanged(PrevState, NewState);
}

void ATPPEquippableItem::DetachMesh()
{
	SkeletalMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void ATPPEquippableItem::AttachToOwner(const FName& SocketName)
{
	const AThirdPersonProjCharacter* CharacterOwner = GetOwnerCharacter();
	check(CharacterOwner);

	SkeletalMesh->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

void ATPPEquippableItem::GrantEquippableAbilities()
{
	if (!ensureAlways(!bHasGrantedAbilties))
	{
		return;
	}
	UAbilitySystemComponent* AbilitySystemComp = GetOwnerAbilitySystem();
	check(AbilitySystemComp);

	if (AbilitiesToGrant.IsValid())
	{
		AbilitiesToGrant.Get()->GiveAbilities_ReturnHandles(AbilitySystemComp, this, GrantedAbilities);
	}

}

void ATPPEquippableItem::RemoveEquippableAbilities()
{
	UTPPAbilitySystemComponent* AbilitySystemComp = Cast<UTPPAbilitySystemComponent>(GetOwnerAbilitySystem());
	if (AbilitySystemComp)
	{
		AbilitySystemComp->ClearAbilities(GrantedAbilities);
	}

	GrantedAbilities.Empty();
}

