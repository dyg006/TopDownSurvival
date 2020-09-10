// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	health = defaultHealth;

}


void UHealthComponent::Init(float maxHealth)
{
	defaultHealth = maxHealth;
	health = defaultHealth;

	alive = true;
}


void UHealthComponent::Damage(float ActualDamage)
{
	health -= ActualDamage;

	if (health <= 0) {
		alive = false;
		health = 0;
	}
}

void UHealthComponent::Heal(float healingPoints)
{
	health = FMath::Min(health + healingPoints, defaultHealth);
}