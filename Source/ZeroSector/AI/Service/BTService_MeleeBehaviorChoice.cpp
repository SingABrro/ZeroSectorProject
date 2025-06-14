// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_MeleeBehaviorChoice.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_MeleeBehaviorChoice::UBTService_MeleeBehaviorChoice()
{
	NodeName = TEXT("Melee Behavior Choice");
	Interval = 1.5f;
}

void UBTService_MeleeBehaviorChoice::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return;

	// 0 >= / < 100
	int32 Probability = FMath::RandRange(0, 100);

	if (Probability >= 0 && Probability < 70)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsCommonAttack"), true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsRushAttack"), false);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsCommonAttack"), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsRushAttack"), true);
	}
}