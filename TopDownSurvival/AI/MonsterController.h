// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TopDownSurvival/TopDownSurvivalGameMode.h"
#include "TopDownSurvival/TopDownSurvivalCharacter.h"
#include "NavigationSystem.h"
#include "MonsterController.generated.h"

class AMonsterCharacter;

/**
 * 
 */
UCLASS()
class TOPDOWNSURVIVAL_API AMonsterController : public AAIController
{
	GENERATED_BODY()

public:
	/// <summary>
	/// Deactivate completely the AI
	/// </summary>
	void DeactivateAI();

	/// <summary>
	/// Checks if the player is perceived.
	/// </summary>
	void CheckPlayerPerceived();
	
protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/// <summary>
	/// The behavior tree
	/// </summary>
	UBehaviorTree* behaviorTree;

	/// <summary>
	/// The blackboard
	/// </summary>
	UBlackboardComponent* blackboard;

private:
	ATopDownSurvivalCharacter* mainCharacter;

	/// <summary>
	/// The monster controlled
	/// </summary>
	AMonsterCharacter* monsterControlled;
	
	/// <summary>
	/// The nav system
	/// </summary>
	UNavigationSystemV1* navSys;

	ATopDownSurvivalGameMode* gameMode;	
};
