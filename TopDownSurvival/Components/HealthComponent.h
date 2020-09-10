// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWNSURVIVAL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	// Apply Damage to Health
	void Damage(float ActualDamage);

	/// <summary>
	/// Heals the specified healing points.
	/// </summary>
	/// <param name="healingPoints">The healing points.</param>
	void Heal(float healingPoints);

	/// <summary>
	/// Gets the health percentage.
	/// </summary>
	/// <returns></returns>
	float GetHealthPercentage() const { return health / defaultHealth; }

	/// <summary>
	/// Initializes the specified maximum health.
	/// </summary>
	/// <param name="maxHealth">The maximum health.</param>
	void Init(float maxHealth);

	/// <summary>
	/// Determines whether [is full health].
	/// </summary>
	/// <returns>
	///   <c>true</c> if [is full health]; otherwise, <c>false</c>.
	/// </returns>
	bool IsFullHealth() const { return defaultHealth == health; }

	/// <summary>
	/// Variable used to established the initial health of the character
	/// </summary>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float defaultHealth;

	// Variable used to established the current health of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
		float health;

	// Variable used to check if the character is alive or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	bool alive;
};
