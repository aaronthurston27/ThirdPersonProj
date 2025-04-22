// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/TPPEquippableItem.h"
#include "TPPEquippableItem_Firearm.generated.h"

UCLASS()
class THIRDPERSONPROJ_API UFirearmProperties_Ammo : public UDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo", meta = (UIMin = "1", ClampMin = "1"))
	int32 ClipCapacity = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 MaxReserveCapacity = ClipCapacity * 3;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(UFirearmProperties_Ammo, ClipCapacity))
		{
			MaxReserveCapacity = ClipCapacity * 3;
		}
		else if (PropertyName == GET_MEMBER_NAME_CHECKED(UFirearmProperties_Ammo, MaxReserveCapacity) && PropertyChangedEvent.ChangeType == EPropertyChangeType::ValueSet)
		{
			if (MaxReserveCapacity < ClipCapacity)
			{
				MaxReserveCapacity = ClipCapacity;
			}
			else
			{
				const int32 Progress = FMath::RoundToInt((float)MaxReserveCapacity / ClipCapacity);
				MaxReserveCapacity = FMath::Max(ClipCapacity * Progress, ClipCapacity);
			}
		}
	}

};

UCLASS()
class THIRDPERSONPROJ_API UFirearmProperties_Shooting : public UDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting", meta = (UIMin = "0.005", ClampMin = "0.005"))
	float TimeBetweenShots = .5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting", meta = (UIMin = '0', ClampMin = '0'))
	int32 AmmoPerShot = 1;
};

UCLASS()
class THIRDPERSONPROJ_API UFirearmProperties_Aiming : public UDataAsset
{

	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	FRuntimeFloatCurve AimInCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aiming")
	FRuntimeFloatCurve AimOutCurve;

};

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API ATPPEquippableItem_Firearm : public ATPPEquippableItem
{
	GENERATED_BODY()
	
protected:

	virtual void GrantEquippableAbilities() override;

	virtual void RemoveEquippableAbilities() override;

	virtual TArray<FGameplayAbilitySpecHandle> GetGrantedAbilities() const;

protected:

	UPROPERTY(Transient)
	FGameplayAbilitySpecHandle ReloadAbilityHandle;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UFirearmProperties_Ammo> AmmoProperties = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UFirearmProperties_Shooting> ShootingProperties = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UFirearmProperties_Aiming> AimingProperties = nullptr;

protected:

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 CurrentClipAmmo = 0;

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 CurrentReserveAmmo = 0;
};
