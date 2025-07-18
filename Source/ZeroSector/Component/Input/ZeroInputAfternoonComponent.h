// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Input/ZeroInputBaseComponent.h"
#include "ZeroInputAfternoonComponent.generated.h"

struct FInputActionValue;
class AZeroGimmick;
class IZeroDialogueInterface;
class UZeroPlayerStatComponent;
UCLASS()
class ZEROSECTOR_API UZeroInputAfternoonComponent : public UZeroInputBaseComponent
{
	GENERATED_BODY()
	
public:
	UZeroInputAfternoonComponent();

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Run() override;
	virtual void Walk() override;

	virtual void InteractBeam() override;
	virtual void InteractProcess(const FHitResult& InHitResult, bool bIsHit) override;
	virtual void DialogueInteract() override;
	virtual void OperationBoardInteract() override;
	virtual void ProvisoInteract() override;
	virtual void EnforceBoardInteract() override;
	virtual void ToggleNote() override;
	virtual void PauseMenu() override;
	virtual void ExcludeResearcher() override;
	virtual void SelectResearcher() override;

	static AActor* CurrentDialogueNPC;

protected:
	virtual void BeginPlay() override;

private:
	void SetDefaultMovement();
	void SetDialogueMovement();
	
	void TryPlayFootstepSound();
	void SetFootstepInterval(float NewInterval);

	void DisplayFKey();

private:
	UPROPERTY(EditAnywhere, Category = "Interact")
	float DetectDistance;
		
	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<AZeroGimmick> InteractedGimmick;

	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<AActor> PrevGimmick;

	UPROPERTY(VisibleAnywhere, Category = "AP")
	TObjectPtr<UZeroPlayerStatComponent> StatComp;

	IZeroDialogueInterface* DialogueInterface;

	bool bIsNoteToggle;
	bool bIsDialogue{ false };

	FTimerHandle FootstepTimerHandle;
	float CurrentFootstepInterval = 0.5f;

};
