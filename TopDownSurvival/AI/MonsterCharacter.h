// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TopDownSurvival/TopDownSurvivalGameMode.h"
#include "TopDownSurvival/Components/HealthComponent.h"
#include "MonsterController.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class TOPDOWNSURVIVAL_API AMonsterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	/// <summary>
	/// Takes the damage.
	/// </summary>
	/// <param name="Damage">The damage.</param>
	/// <param name="DamageEvent">The damage event.</param>
	/// <param name="EventInstigator">The event instigator.</param>
	/// <param name="DamageCauser">The damage causer.</param>
	/// <returns></returns>
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	// Damage sigle actor
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void CausePainTo(class AActor* enemy, bool useDistance = false);
	
	/* Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	/// <summary>
	/// The health component
	/// </summary>
	UHealthComponent* healthComponent;

	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Returns if the enemy dead or no.
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() const { return dead; }

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The damage deal to the player
	/// </summary>
	float playerDamage = 15.f;

	/// <summary>
	/// Attacks the specified objective.
	/// </summary>
	/// <param name="objective">The objective.</param>
	void Attack(AActor* objective);

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	/// <summary>
	/// The objective to attack
	/// </summary>
	AActor* attackObjective;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	/// <summary>
	/// The health bar widget
	/// </summary>
	UWidgetComponent* healthBarWidget;

	UFUNCTION(BlueprintImplementableEvent)
	/// <summary>
	/// Updates the health bar of the enemy so it does not get updated every frame from the widget.
	/// </summary>
	void UpdateHealthBar();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// The common enemy character animation
	/// </summary>
	UAnimInstance* enemyAnimationInstance;
	
	/// <summary>
	/// Attack anim montage of the common monster character
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "EnemyAnimationMontage")
	class UAnimMontage* attackAnimMontage;
	
	/// <summary>
	/// Enemy skeletal mesh component
	/// </summary>
	USkeletalMeshComponent* enemyMeshComponent = nullptr;
	
private:	
	/// <summary>
	/// The game mode
	/// </summary>
	ATopDownSurvivalGameMode* gameMode;

	/// <summary>
	/// Kills the monster.
	/// </summary>
	void KillMonster();

	/// <summary>
	/// Destroys the actor itself.
	/// </summary>
	void DestroySelf();
	
	/// <summary>
	/// Deactivates the ai movement.
	/// </summary>
	void DeactivateAIMovement();

	/// <summary>
	/// Gets the distance to an enemy.
	/// </summary>
	/// <param name="enemy">The enemy.</param>
	/// <returns></returns>
	float GetDistToEnemy(AActor* enemy) const;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The attack range of the enemy
	/// </summary>
	float attackRange = 125.f;
	
	/// <summary>
	/// The character movement component
	/// </summary>
	class UCharacterMovementComponent* characterMovement;

	/// <summary>
	/// Check if the monster is dead
	/// </summary>
	bool dead = false;

	/// <summary>
	/// The death delay
	/// </summary>
	const float deathDelay = 3.f;

	/// <summary>
	/// The timer handle used in order to wait until enemy destruction
	/// </summary>
	FTimerHandle timerHandle;

	/// <summary>
	/// The monster controller
	/// </summary>
	AMonsterController* monsterController;
};
