// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ZeroGimmick.h"
#include "ZeroOperationBoard.generated.h"

class UBoxComponent;
class UWidgetComponent;

UCLASS()
class ZEROSECTOR_API AZeroOperationBoard : public AZeroGimmick
{
	GENERATED_BODY()
	
public:	
	AZeroOperationBoard();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void AccordingToDayOpBoardSelect() const;
	void SetUIDisplay();
	void SetUIUnDisplay(const FVector& InVector);

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UWidgetComponent> WidgetComp;

	UPROPERTY(EditAnywhere, Category = "Start")
	uint8 StartDay;

	float WidgetYawRotation{ 0.0f };
};
