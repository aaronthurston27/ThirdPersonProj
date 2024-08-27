// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/TPPAnimNotify_BoneTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UE_DEFINE_GAMEPLAY_TAG(BONE_TRACE_TAG, "GameplayEvent.BoneTraceNotify");

void UTPPAnimNotify_BoneTrace::PostLoad()
{
	Super::PostLoad();
}

void UTPPAnimNotify_BoneTrace::PostEditChangeProperty(FPropertyChangedEvent& Prop)
{
	Super::PostEditChangeProperty(Prop);
	const FName PropertyName = Prop.Property != nullptr ? Prop.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UTPPAnimNotify_BoneTrace, bIsCapsuleCollision))
	{
		if (bIsCapsuleCollision)
		{
			bIsBoxCollision = false;
			bIsSphereCollision = false;
		}

		bCanEditRadiusProperty = bIsSphereCollision || bIsCapsuleCollision;
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UTPPAnimNotify_BoneTrace, bIsSphereCollision))
	{
		if (bIsSphereCollision)
		{
			bIsBoxCollision = false;
			bIsCapsuleCollision = false;
		}

		bCanEditRadiusProperty = bIsSphereCollision || bIsCapsuleCollision;
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UTPPAnimNotify_BoneTrace, bIsBoxCollision))
	{
		if (bIsBoxCollision)
		{
			bIsCapsuleCollision = false;
			bIsSphereCollision = false;
		}

		bCanEditRadiusProperty = bIsSphereCollision || bIsCapsuleCollision;
	}
}

void UTPPAnimNotify_BoneTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UTPPAnimNotify_BoneTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (MeshComp->GetBoneIndex(BoneName) != -1)
	{
		const UWorld* World = MeshComp->GetWorld();
		const FTransform BoneTransform_World = MeshComp->GetBoneTransform(BoneName);

		const FVector TraceCenterPosition = BoneTransform_World.TransformPosition(LocalSpaceOffset);
		const FRotator BoneRotation = BoneTransform_World.Rotator();
		const FRotator TraceRotation = bIsCapsuleCollision ? UKismetMathLibrary::MakeRotFromZ(BoneRotation.Vector()) : UKismetMathLibrary::MakeRotFromX(BoneRotation.Vector());
		const FVector TraceEndPosition = TraceCenterPosition + (BoneRotation.Vector() * BoneTraceForwardDistance);

		if (bDrawDebug)
		{
			if (bIsCapsuleCollision)
			{
				DrawDebugCapsule(World, TraceCenterPosition, BoneTraceCapsuleHeight, BoneTraceRadius, TraceRotation.Quaternion(), DebugDrawColor, false, DebugDrawDuration, 0, DebugDrawThickness);
			}
			else if (bIsBoxCollision)
			{
				DrawDebugBox(World, TraceCenterPosition, BoxCollisionTraceExtents, DebugDrawColor, false, DebugDrawDuration, 0, DebugDrawThickness);
			}
			else if (bIsSphereCollision)
			{
				DrawDebugSphere(World, TraceCenterPosition, BoneTraceRadius, 13, DebugDrawColor, false, DebugDrawDuration, 0, DebugDrawThickness);
			}
		}

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MeshComp->GetOwner());
		const FCollisionShape CollisionShape = bIsCapsuleCollision ? FCollisionShape::MakeCapsule(BoneTraceRadius, BoneTraceCapsuleHeight) : 
			bIsBoxCollision ? FCollisionShape::MakeBox(BoxCollisionTraceExtents) : FCollisionShape::MakeSphere(BoneTraceRadius);
		TArray<FHitResult> HitResults;

		World->SweepMultiByChannel(HitResults, TraceCenterPosition, TraceEndPosition, TraceRotation.Quaternion(), CollisionChannel, CollisionShape, QueryParams);

		for (const FHitResult& HitResult : HitResults)
		{
			AActor* Owner = Cast<AActor>(MeshComp->GetOwner());
			if (Owner)
			{
				FGameplayEventData NotifyEventData;
				NotifyEventData.EventTag = BONE_TRACE_TAG;
				NotifyEventData.Instigator = Owner;
				NotifyEventData.Target = HitResult.GetActor();
				NotifyEventData.ContextHandle.AddHitResult(HitResult);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, BONE_TRACE_TAG, NotifyEventData);
			}
		}
	}
}

void UTPPAnimNotify_BoneTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
