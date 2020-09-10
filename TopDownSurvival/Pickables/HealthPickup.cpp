// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"

#include "FMODBlueprintStatics.h"
#include "../Components/HealthComponent.h"
#include "TopDownSurvival/TopDownSurvivalCharacter.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = healthParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	sphereCollision->SetupAttachment(healthParticle);
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHealthPickup::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	ATopDownSurvivalCharacter* mainCharacter = Cast<ATopDownSurvivalCharacter>(otherActor);
	if(mainCharacter != nullptr)
	{
		if(!mainCharacter->healthComponent->IsFullHealth())
		{			
			if (healthPickupSound != nullptr) {
				UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), healthPickupSound, GetActorTransform(), true);
			}
			mainCharacter->healthComponent->Heal(healthToRestore);
			mainCharacter->UpdateHeartbeatSound();
			mainCharacter->DeactivateDamageShader();
			Destroy();
		}
	}
}


