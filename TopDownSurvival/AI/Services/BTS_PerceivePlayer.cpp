// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_PerceivePlayer.h"
#include "AIController.h"
#include "../MonsterController.h"

void UBTS_PerceivePlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//Llamamos a la implementación de la clase base primero
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Obtenemos la referencia del MonsterController
	AMonsterController* monsterController = Cast<AMonsterController>(OwnerComp.GetOwner());

	//Metodo CheckPlayerPerceived del monsterController que tiene toda la lógica para determinar si el
	// jugador está cerca o no y configurar el Key correspontiente del Blackboard
	monsterController->CheckPlayerPerceived();
}