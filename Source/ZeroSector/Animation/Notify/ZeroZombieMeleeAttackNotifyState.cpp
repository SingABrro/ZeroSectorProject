// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/ZeroZombieMeleeAttackNotifyState.h"
#include "Interface/ZeroCharacterAIInterface.h"
#include "GameFramework/Character.h"
#include "Engine/DamageEvents.h"
#include "ZeroSector.h"

UZeroZombieMeleeAttackNotifyState::UZeroZombieMeleeAttackNotifyState()
{
}

void UZeroZombieMeleeAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UZeroZombieMeleeAttackNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (Character)
	{
		MakeSphereTrace(Character);
	}
}

void UZeroZombieMeleeAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	HitTarget.Empty();
}

void UZeroZombieMeleeAttackNotifyState::MakeSphereTrace(ACharacter* Character)
{
	// 본에 소켓 추가 Hand_L, Hand_R
	FVector LeftHandLoc = Character->GetMesh()->GetSocketLocation(TEXT("hand_lSocket"));
	FRotator LeftHandRot = Character->GetMesh()->GetSocketRotation(TEXT("hand_lSocket"));

	FVector RightHandLoc = Character->GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
	FRotator RightHandRot = Character->GetMesh()->GetSocketRotation(TEXT("hand_rSocket"));

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, true, Character);

	bool bLeftHandHit = Character->GetWorld()->SweepSingleByChannel(HitResult, LeftHandLoc, LeftHandLoc, LeftHandRot.Quaternion(), ECC_GameTraceChannel2, FCollisionShape::MakeBox(FVector(50.f)), Params);
	bool bRightHandHit = Character->GetWorld()->SweepSingleByChannel(HitResult, RightHandLoc, RightHandLoc, RightHandRot.Quaternion(), ECC_GameTraceChannel2, FCollisionShape::MakeBox(FVector(50.f)), Params);

	DrawDebugBox(Character->GetWorld(), LeftHandLoc, FVector(), FColor::Red, false);
	DrawDebugBox(Character->GetWorld(), RightHandLoc, FVector(), FColor::Red, false);

	if ((bLeftHandHit || bRightHandHit) && !HitTarget.Contains(HitResult.GetActor()))
	{
		HitTarget.Add(HitResult.GetActor());

		FDamageEvent DamageEvent;
		IZeroCharacterAIInterface* Interface = Cast<IZeroCharacterAIInterface>(Character);
		if (Interface)
		{
			if (HitResult.GetActor())
			{
				HitResult.GetActor()->TakeDamage(300.f, DamageEvent, Interface->GetAIController(), Character);
			}
		}
	}
}
