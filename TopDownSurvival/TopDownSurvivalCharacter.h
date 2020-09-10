// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Materials/MaterialParameterCollection.h"
#include "TopDownSurvivalGameMode.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/Weapon.h"
#include "TopDownSurvivalCharacter.generated.h"

class ATopDownSurvivalPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthStatusDelegate, float, currentHealth);

UCLASS(Blueprintable)
class ATopDownSurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownSurvivalCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	/// <summary>
	/// Starts firing the weapon.
	/// </summary>
	void StartFiringWeapon();
	
	/// <summary>
	/// Stops firing the weapon.
	/// </summary>
	void StopFiringWeapon();

	/* Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* healthComponent;

	/// <summary>
	/// Takes the damage.
	/// </summary>
	/// <param name="Damage">The damage.</param>
	/// <param name="DamageEvent">The damage event.</param>
	/// <param name="EventInstigator">The event instigator.</param>
	/// <param name="DamageCauser">The damage causer.</param>
	/// <returns></returns>
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	//Used to allow the vibration of the controller
	UPROPERTY(EditAnywhere, Category = "Character")
	class UForceFeedbackEffect* playerHurt;

	//event dispatcher
	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FHealthStatusDelegate healthStatusEvent;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// Check if the player is outside the mission area
	/// </summary>
	bool outsideMissionArea = false;

	UPROPERTY(EditAnywhere, Category = "Sound")
	//The firing component sound
	class UFMODAudioComponent* firingComponentSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	/// <summary>
	/// The hurt component sound
	/// </summary>
	class UFMODAudioComponent* hurtComponentSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	/** Heartbeat sound component of the character */
	class UFMODAudioComponent* heartbeatSound;

	/// <summary>
	/// Updates the heartbeat sound.
	/// </summary>
	void UpdateHeartbeatSound();

	/// <summary>
	/// Resets the health character parameters.
	/// </summary>
	void ResetCharacterParams();

	/// <summary>
	/// The instance of the material parameter collection
	/// </summary>
	UMaterialParameterCollectionInstance* collectionInstance;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The parameter collection of a material
	/// </summary>
	UMaterialParameterCollection* parameterCollection;

	/// <summary>
	/// Activates the damage shader.
	/// </summary>
	void ActivateDamageShader();
	
	/// <summary>
	/// Deactivates the damage shader.
	/// </summary>
	void DeactivateDamageShader();
	
protected:
	void BeginPlay();
	
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	/// <summary>
	/// The weapon equipped
	/// </summary>
	UPROPERTY(EditAnywhere, Category = "Weapon")
	AWeapon* weaponEquipped = nullptr;

	/// <summary>
	/// Trace the raycast of the shot.
	/// </summary>
	void WeaponTrace();

	/// <summary>
	/// Gets the weapon trace start location.
	/// </summary>
	/// <returns></returns>
	FVector GetWeaponTraceStartLocation();
	
	/// <summary>
	/// Gets the weapon trace end location.
	/// </summary>
	/// <returns></returns>
	FVector GetWeaponTraceEndLocation();

	/// <summary>
	/// The timer handle used in order to fire the weapon on loop within a time range
	/// </summary>
	FTimerHandle timerHandle;

	/// <summary>
	/// The game mode
	/// </summary>
	ATopDownSurvivalGameMode* gameMode;

	/// <summary>
	/// The player controller
	/// </summary>
	ATopDownSurvivalPlayerController* playerController;

	/// <summary>
	/// The low health percentage in order to activate the blood shader
	/// </summary>
	const float lowHealthPercentage = 0.4f;
};

