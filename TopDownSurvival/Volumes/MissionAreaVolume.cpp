// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionAreaVolume.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownSurvival/TopDownSurvivalCharacter.h"

// Sets default values
AMissionAreaVolume::AMissionAreaVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	
}

// Called when the game starts or when spawned
void AMissionAreaVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionAreaVolume::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	ATopDownSurvivalCharacter* mainCharacter = Cast<ATopDownSurvivalCharacter>(otherActor);
	if (mainCharacter != nullptr)
	{
		mainCharacter->outsideMissionArea = false;
		GetWorldTimerManager().ClearTimer(deathTimerHandle);
	}
}


void AMissionAreaVolume::NotifyActorEndOverlap(AActor* otherActor)
{
	Super::NotifyActorEndOverlap(otherActor);

	ATopDownSurvivalCharacter* mainCharacter = Cast<ATopDownSurvivalCharacter>(otherActor);
	if (mainCharacter != nullptr)
	{
		mainCharacter->outsideMissionArea = true;
		GetWorldTimerManager().SetTimer(deathTimerHandle, this, &AMissionAreaVolume::KillPlayer, secondsBeforePlayerDeath, false);
	}

}

void AMissionAreaVolume::KillPlayer()
{
	ATopDownSurvivalCharacter* mainCharacter = Cast<ATopDownSurvivalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (mainCharacter != nullptr)
	{
		// Create a damage event  
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		AController* damageInstigatorController = GetInstigatorController();
		mainCharacter->TakeDamage(mainCharacter->healthComponent->defaultHealth, DamageEvent, damageInstigatorController, this);
	}
}