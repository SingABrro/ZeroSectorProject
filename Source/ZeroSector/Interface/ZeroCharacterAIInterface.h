// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroCharacterAIInterface.generated.h"

DECLARE_DELEGATE(FOnAttackOneFinished)
DECLARE_DELEGATE(FOnAttackTwoFinished)
DECLARE_DELEGATE(FOnSpawnAttackFinished)

UINTERFACE(MinimalAPI)
class UZeroCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroCharacterAIInterface
{
	GENERATED_BODY()

public:
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
	virtual float GetRunSpeed() = 0;
	virtual float GetWalkSpeed() = 0;

	virtual void SetAIAttackOneDelegate(const FOnAttackOneFinished& InOnAttackOneFinished) = 0;
	virtual void SetAIAttackTwoDelegate(const FOnAttackTwoFinished& InOnAttackTwoFinished) = 0;
	virtual void SetAISpawnAttackDelegate(const FOnSpawnAttackFinished& InOnSpawnAttackFinished) { }

	virtual void AttackOneByAI() = 0;
	virtual void AttackTwoByAI() = 0;
	virtual void SpawnAttackByAI() { }

	virtual AController* GetAIController() = 0;
};
