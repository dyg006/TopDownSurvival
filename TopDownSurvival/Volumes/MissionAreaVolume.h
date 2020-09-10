// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MissionAreaVolume.generated.h"

UCLASS()
class TOPDOWNSURVIVAL_API AMissionAreaVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionAreaVolume();

	UBoxComponent* boxCollision;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/// <summary>
	/// Notifies the actor begin overlap.
	/// </summary>
	/// <param name="otherActor">The other actor.</param>
	void NotifyActorBeginOverlap(AActor* otherActor) override;

	/// <summary>
	/// Notifies the actor end overlap.
	/// </summary>
	/// <param name="otherActor">The other actor.</param>
	void NotifyActorEndOverlap(AActor* otherActor) override;
	
	/// <summary>
	/// Kills the player instantly.
	/// </summary>
	void KillPlayer();

	/// <summary>
	/// The seconds before player death
	/// </summary>
	float secondsBeforePlayerDeath = 2.f;

	/// <summary>
	/// The timer handle used in order to wait until enemy destruction
	/// </summary>
	FTimerHandle deathTimerHandle;
};
