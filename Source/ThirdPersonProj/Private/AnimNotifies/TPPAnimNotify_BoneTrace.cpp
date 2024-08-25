// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/TPPAnimNotify_BoneTrace.h"
#include "Kismet/KismetMathLibrary.h"

void UTPPAnimNotify_BoneTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) 
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UTPPAnimNotify_BoneTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp->GetBoneIndex(BoneName) != -1)
	{
		const FTransform BoneTransform_World = MeshComp->GetBoneTransform(BoneName);

		const FVector TraceCenterPosition = BoneTransform_World.TransformPosition(LocalSpaceOffset);
		const FRotator BoneRotation = BoneTransform_World.Rotator();
		const FRotator CapsuleRotation = UKismetMathLibrary::MakeRotFromZ(BoneRotation.Vector());

		if (bDrawDebug)
		{
			DrawDebugCapsule(MeshComp->GetWorld(), TraceCenterPosition, BoneTraceCapsuleHeight, BoneTraceCapsuleRadius, CapsuleRotation.Quaternion(), DebugDrawColor, false, DebugDrawDuration, 0, DebugDrawThickness);
		}
	}
}

void UTPPAnimNotify_BoneTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
