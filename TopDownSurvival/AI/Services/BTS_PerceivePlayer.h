// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_PerceivePlayer.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNSURVIVAL_API UBTS_PerceivePlayer : public UBTService
{
	GENERATED_BODY()

public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
