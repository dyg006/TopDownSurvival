// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = skeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	//skeletalMesh->SetupAttachment(RootComponent);

	weaponParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	weaponParticles->SetupAttachment(skeletalMesh);
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ActivateParticles()
{
	if(weaponParticles != nullptr)
	{
		weaponParticles->Activate(false);
	}
}

void AWeapon::DeactivateParticles()
{
	if(weaponParticles != nullptr)
	{
		weaponParticles->Deactivate();
	}
}

FVector AWeapon::GetWeaponSocketLocation(FName socketName) const
{
	if(skeletalMesh == nullptr)
	{
		return FVector::ZeroVector;
	}
	return skeletalMesh->GetSocketLocation(socketName);
}


void AWeapon::SpawnParticlesCollision(FVector location)
{
	if(particlesCollision != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), particlesCollision, location);
	}	
}

void AWeapon::CauseDamageTo(AActor* enemy, const FHitResult& hit)
{
	if (damage <= 0.f)
	{
		return;
	}

	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = hit;
	AController* damageInstigatorController = GetInstigatorController();
	enemy->TakeDamage(damage, DamageEvent, damageInstigatorController, this);
}