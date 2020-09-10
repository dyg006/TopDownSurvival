// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSURVIVAL_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:

	/// <summary>
	/// Executes the task.
	/// </summary>
	/// <param name="OwnerComp">The owner comp.</param>
	/// <param name="NodeMemory">The node memory.</param>
	/// <returns>Returns Succeded, Failed or In Progress</returns>
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	/// <summary>
	/// The objective to attack
	/// </summary>
	FBlackboardKeySelector attackObjective;

};
