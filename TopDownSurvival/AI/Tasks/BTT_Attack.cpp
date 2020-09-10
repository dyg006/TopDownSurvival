// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../MonsterCharacter.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* objective = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(attackObjective.SelectedKeyName));

	Cast<AMonsterCharacter>(OwnerComp.GetAIOwner()->GetPawn())->Attack(objective);

	return EBTNodeResult::Succeeded;
}
