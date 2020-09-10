// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "MonsterController.h"
#include "TopDownSurvival/TopDownSurvivalCharacter.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthEnemyComponent"));
	healthComponent->Init(100);

	healthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	healthBarWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	gameMode = Cast<ATopDownSurvivalGameMode>(GetWorld()->GetAuthGameMode());
	characterMovement = GetCharacterMovement();
	enemyMeshComponent = GetMesh();
	monsterController = Cast<AMonsterController>(GetController());	
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMonsterCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	//float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamage = Damage;

	// DamageType example
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();

	// Health component
	if (ActualDamage != 0.f)
	{
		FPointDamageEvent* const oPointDamageEvent = (FPointDamageEvent*)&DamageEvent;

		FName boneHit = oPointDamageEvent->HitInfo.BoneName;
		
		// Apply the damage amount
		healthComponent->Damage(ActualDamage);
		UpdateHealthBar();
		// if dead destroy
		if (!healthComponent->alive) {
			
			//generic death
			KillMonster();

			GetWorldTimerManager().SetTimer(timerHandle, this, &AMonsterCharacter::DestroySelf, deathDelay, false);
		}
	}

	return ActualDamage;
}

void AMonsterCharacter::KillMonster()
{
	dead = true;
	if(monsterController != nullptr)
	{
		monsterController->DeactivateAI();
	}
	DeactivateAIMovement();
	SetActorEnableCollision(false);
	if(gameMode != nullptr)
	{
		gameMode->RegisterMonsterDeath(this);
	}
}

void AMonsterCharacter::DeactivateAIMovement()
{
	if(characterMovement != nullptr)
	{
		characterMovement->StopMovementImmediately();
		characterMovement->Deactivate();
	}
}

void AMonsterCharacter::DestroySelf()
{
	GetWorldTimerManager().ClearTimer(timerHandle);
	Destroy();
}

void AMonsterCharacter::CausePainTo(AActor* enemy, bool useDistance)
{

	if (playerDamage <= 0.f)
	{
		return;
	}
	if (useDistance && GetDistToEnemy(enemy) > attackRange)
	{
		return;
	}

	// Create a damage event  
	TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
	FDamageEvent DamageEvent(ValidDamageTypeClass);
	AController* damageInstigatorController = GetInstigatorController();
	enemy->TakeDamage(playerDamage, DamageEvent, damageInstigatorController, this);

}

float AMonsterCharacter::GetDistToEnemy(AActor* enemy) const
{
	float distance = FVector::Dist(enemy->GetActorLocation(), GetActorLocation());
	distance -= Cast<ATopDownSurvivalCharacter>(enemy)->GetCapsuleComponent()->GetScaledCapsuleRadius();
	return distance;
}

void AMonsterCharacter::Attack(AActor* objective) {
	if (enemyMeshComponent != nullptr) {
		enemyAnimationInstance = enemyMeshComponent->GetAnimInstance();

		if (!enemyAnimationInstance->Montage_IsPlaying(attackAnimMontage)) {
			attackObjective = objective;
			enemyAnimationInstance->Montage_Play(attackAnimMontage);
		}
	}
}
