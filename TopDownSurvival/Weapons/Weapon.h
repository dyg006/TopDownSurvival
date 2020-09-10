// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODStudioModule.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon.generated.h"

UCLASS()
class TOPDOWNSURVIVAL_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Activates the particles.
	/// </summary>
	void ActivateParticles();
	
	/// <summary>
	/// Deactivates the particles.
	/// </summary>
	void DeactivateParticles();

	/// <summary>
	/// Gets the weapon socket location.
	/// </summary>
	/// <param name="socketName">Name of the socket.</param>
	/// <returns></returns>
	FVector GetWeaponSocketLocation(FName socketName) const;

	/// <summary>
	/// Gets the weapon range.
	/// </summary>
	/// <returns></returns>
	float GetWeaponRange()    const { return weaponRange; }

	/// <summary>
	/// Gets the weapon fire rate.
	/// </summary>
	/// <returns></returns>
	float GetWeaponFireRate() const { return weaponFireRate; }

	/// <summary>
	/// Causes the damage to.
	/// </summary>
	/// <param name="enemy">The enemy.</param>
	/// <param name="hit">The hit.</param>
	void CauseDamageTo(AActor* enemy, const FHitResult& hit);
	
	/// <summary>
	/// Spawns the particles collision.
	/// </summary>
	/// <param name="location">The location.</param>
	void SpawnParticlesCollision(FVector location);

	UPROPERTY(EditAnywhere, Category = "Weapon")
		/// <summary>
		/// The skeletal mesh
		/// </summary>
		USkeletalMeshComponent* skeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "WeaponParticles")
		/// <summary>
		/// The weapon particles
		/// </summary>
		UParticleSystemComponent* weaponParticles;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The item picked sound
	/// </summary>
	UFMODEvent* firingSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	/// <summary>
	/// The weapon range
	/// </summary>
	float weaponRange = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	/// <summary>
	/// The weapon fire rate
	/// </summary>
	float weaponFireRate = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	/// <summary>
	/// The damage
	/// </summary>
	float damage = 0.f;
	
private:
	
	UPROPERTY(EditAnywhere, Category = "WeaponParticles")
		/// <summary>
		/// The particles collision with environment
		/// </summary>
		UParticleSystem* particlesCollision;
};
