// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController.h"
#include "MonsterCharacter.h"
#include "Kismet/GameplayStatics.h"


void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	if (behaviorTree == nullptr)
	{
		return;
	}

	RunBehaviorTree(behaviorTree);

	blackboard = GetBlackboardComponent();

	if (blackboard == nullptr)
	{
		return;
	}

	mainCharacter = Cast<ATopDownSurvivalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	blackboard->SetValueAsObject("EnemyActor", mainCharacter);
	blackboard->SetValueAsBool("Active", true);

	navSys = UNavigationSystemV1::GetCurrent(GetWorld());

	monsterControlled = Cast<AMonsterCharacter>(GetPawn());
	gameMode = Cast<ATopDownSurvivalGameMode>(GetWorld()->GetAuthGameMode());
}

void AMonsterController::DeactivateAI()
{
	blackboard->SetValueAsBool("Active", false);
}


void AMonsterController::CheckPlayerPerceived()
{
	if(gameMode != nullptr && gameMode->GetIsPlayerDead() && blackboard->GetValueAsBool("Active"))
	{
		DeactivateAI();
	}
}