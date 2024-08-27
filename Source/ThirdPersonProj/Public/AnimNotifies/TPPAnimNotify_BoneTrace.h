// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NativeGameplayTags.h"
#include "TPPAnimNotify_BoneTrace.generated.h"

UE_DECLARE_GAMEPLAY_TAG_EXTERN(BONE_TRACE_TAG);

/**
 * 
 */
UCLASS()
class THIRDPERSONPROJ_API UTPPAnimNotify_BoneTrace : public UAnimNotifyState
{
	GENERATED_BODY()

public:

	virtual void PostLoad() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& Prop) override;

protected:

	static const FName BoneTraceEventName;

	UPROPERTY(EditAnywhere)
	FName BoneName = FName(TEXT("None"));

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECollisionChannel::ECC_WorldDynamic;

	UPROPERTY(EditAnywhere)
	bool bIsCapsuleCollision = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsCapsuleCollision"))
	float BoneTraceCapsuleHeight = 10.0f;

	UPROPERTY(EditAnywhere)
	bool bIsSphereCollision = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bCanEditRadiusProperty"))
	float BoneTraceRadius = 20.0f;

	UPROPERTY(EditAnywhere)
	bool bIsBoxCollision = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bIsBoxCollision"))
	FVector BoxCollisionTraceExtents = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float BoneTraceForwardDistance = 2.0f;

	UPROPERTY(EditAnywhere)
	FVector LocalSpaceOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bDrawDebug"))
	float DebugDrawDuration = .8f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bDrawDebug"))
	float DebugDrawThickness = 1.0f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "bDrawDebug"))
	FColor DebugDrawColor = FColor::Green;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:

	bool bCanEditRadiusProperty = false;
};
