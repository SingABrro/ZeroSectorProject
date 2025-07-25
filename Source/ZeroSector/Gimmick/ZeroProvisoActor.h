// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ZeroGimmick.h"
#include "Data/ZeroProvisoType.h"
#include "ZeroProvisoActor.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroProvisoActor : public AZeroGimmick
{
	GENERATED_BODY()
	
public:	
	AZeroProvisoActor();


protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Proviso")
	EZeroProvisoType ProvisoType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Proviso")
	FName RowName;


};
