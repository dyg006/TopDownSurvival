// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

UCLASS()
class TOPDOWNSURVIVAL_API AHealthPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPickup();

	UPROPERTY(EditAnywhere)
	float healthToRestore = 100.f;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystemComponent* healthParticle;

	UPROPERTY(EditAnywhere, Category = "Collision")
	USphereComponent* sphereCollision;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The health pickup sound
	/// </summary>
	UFMODEvent* healthPickupSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	/// <summary>
	/// Notifies the actor begin overlap.
	/// </summary>
	/// <param name="otherActor">The other actor.</param>
	void NotifyActorBeginOverlap(AActor* otherActor) override;
};
