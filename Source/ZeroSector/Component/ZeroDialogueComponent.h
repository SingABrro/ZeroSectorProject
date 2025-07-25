// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/ZeroDialogueInterface.h"
#include "Data/ZeroDialogueDataTable.h"
#include "Character/ZeroNPCType.h"
#include "ZeroDialogueComponent.generated.h"

struct FZeroDialogueOptionDataTable;
class UZeroDialogueWidget;
class UZeroDialogueOptionWidget;
class UZeroResearcherData;
class ACharacter;
class AZeroAIControllerNPC;
class AZeroPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROSECTOR_API UZeroDialogueComponent 
	: public UActorComponent
	, public IZeroDialogueInterface
{
	GENERATED_BODY()

public:	
	UZeroDialogueComponent();

	/* IZeroDialogueInterface 구현 */
	virtual void StartDialogue() override;
	virtual void SetupFinishedDialogueDelegate(const FOnFinishedDialogue& InOnFinishedDialogue) override;

protected:
	virtual void BeginPlay() override;

public:
	/* Dialogue Option Widget 에서 버튼 클릭 시 호출하는 함수 */
	void OnClickedOption(FZeroDialogueDataTable InDialogueTable, float Reliability);

	/* Editor 에서 실행한 후 데이터 초기화 해주는 함수 */
	void ShutdownGame();

/* Util */
private:
	AZeroPlayerController* GetPlayerController() const;

/* Owner Actor Rotation */
private:
	void RotationToPlayer();

/* Input Mode */
private:
	void InputModeGameOnly();
	void InputModeGameAndUI();
	void InputModeUIOnly();

/* 대화 중일 때 호출되는 함수 */
private:
	void InProgressDialogue();

/* 다음 날로 넘어가는 함수 */
private:
	void NextDayDialogue(uint8 InDay);

/* 인터뷰로 넘어가는 함수 */
private:
	void StartInterviewDialogue(uint8 InReliabilityLevel);

/* Owner */
private:
	AZeroAIControllerNPC* GetAIController() const;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<ACharacter> Character;


/* UI */
private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroDialogueWidget> DialogueWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroDialogueWidget> DialogueWidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroDialogueOptionWidget> DialogueOptionWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroDialogueOptionWidget> DialogueOptionWidgetPtr;

/* Flag */
private:
	bool bIsInterview = false;
	bool bIsTalking = false;

/* Dialogue Data Info */
private:
	void DialogueOptionSpawn(const FZeroDialogueOptionDataTable& InDialogueOptionTable);

	void DialogueDataInit();
	void DialogueOptionDataInit(TSoftObjectPtr<UDataTable> InDataTable, FName InRowIndex);

	UPROPERTY(EditAnywhere, Category = "Name")
	FText ActorName;

	UPROPERTY(VisibleAnywhere)
	FName RowIndex;

	UPROPERTY(VisibleAnywhere)
	FZeroDialogueDataTable DialogueTable;

	UPROPERTY(VisibleAnywhere)
	bool bIsEnd;

	UPROPERTY(VisibleAnywhere)
	FName PrevIndex;

	UPROPERTY(EditAnywhere, Category = "Level")
	uint8 ReliabilityLevel;

	FOnFinishedDialogue OnFinishedDialogue;

	TSoftObjectPtr<UDataTable> P_DialogueTable;
	TSoftObjectPtr<UDataTable> S_DialogueTable;
	TSoftObjectPtr<UDataTable> V_DialogueTable;
	TSoftObjectPtr<UDataTable> C_DialogueTable;
	TSoftObjectPtr<UDataTable> N1_DialogueTable;
	TSoftObjectPtr<UDataTable> N2_DialogueTable;
	TSoftObjectPtr<UDataTable> N3_DialogueTable;
	TMap<FName, TSoftObjectPtr<UDataTable>> DialogueTableMap;

	TSoftObjectPtr<UZeroResearcherData> V_Researcher;
	TSoftObjectPtr<UZeroResearcherData> C_Researcher;
	TSoftObjectPtr<UZeroResearcherData> N1_Researcher;
	TSoftObjectPtr<UZeroResearcherData> N2_Researcher;
	TSoftObjectPtr<UZeroResearcherData> N3_Researcher;
	TMap<FName, TSoftObjectPtr<UZeroResearcherData>> ResearcherDataMap;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UZeroResearcherData> ResearcherData;

	TMap<FName, FString> NameTable;
};
