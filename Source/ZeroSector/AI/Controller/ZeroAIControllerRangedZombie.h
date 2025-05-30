// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/ZeroAIControllerBase.h"
#include "ZeroAIControllerRangedZombie.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

UCLASS()
class ZEROSECTOR_API AZeroAIControllerRangedZombie : public AZeroAIControllerBase
{
	GENERATED_BODY()

public:
	AZeroAIControllerRangedZombie();

	virtual bool IsDead() const override;

protected:
	virtual void BeginPlay() override;

	
};