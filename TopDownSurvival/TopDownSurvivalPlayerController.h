// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TopDownSurvivalGameMode.h"
#include "GameFramework/PlayerController.h"
#include "TopDownSurvivalPlayerController.generated.h"

class ATopDownSurvivalCharacter;

UCLASS()
class ATopDownSurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATopDownSurvivalPlayerController();

	/// <summary>
	/// The input mode used by the controller
	/// </summary>
	FInputModeGameAndUI inputModeGameAndUI;

	/// <summary>
	/// The input mode used by the controller
	/// </summary>
	FInputModeGameOnly inputModeGameOnly;

	/// <summary>
	/// Updates the character rotation.
	/// </summary>
	void UpdateCharacterRotation(const FVector cursorPosition);

	/// <summary>
	/// Gets if the is gamepad active.
	/// </summary>
	/// <returns></returns>
	bool GetIsGamepadActive() { return isGamepadActive; }
	
protected:
	void BeginPlay();
	
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	//Allows character to receive the move input
	void MoveForward(float value);
	void MoveRight(float value);

	//Allows character to aim in different directions
	void AimForward(float value);
	void AimRight(float value);

	//Allows character to receive fire input
	void StartFiringWeapon();
	void StopFiringWeapon();

	/* [Start Button || Esc ] Pause the game */
	void Pause();
	
	//Check if any key has been pressed
	void AnyKeyPressed(FKey key);
	
	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	
private:
	/// <summary>
	/// The current pawn
	/// </summary>
	APawn* currentPawn;

	/// <summary>
	/// The thumbstick deadzone in which the character can start aiming
	/// </summary>
	const float thumbstickDeadzone = 0.5f;
	/// <summary>
	/// The mouse deadzone in which the character can start aiming
	/// </summary>
	const float mouseDeadzone = 20.f;

	/// <summary>
	/// The mouse sensitivity
	/// </summary>
	const float mouseSensitivity = 7.f;

	//check if gamepad is active
	bool isGamepadActive = false;

	//check if thumbstick joystick is above the threshold in order to trigger the aim
	bool IsThumbstickAboveDeadzone();
	//check if thumbstick mouse is above the threshold in order to trigger the aim
	bool IsMouseAboveDeadzone();

	/// <summary>
	/// Gets the mouse velocity.
	/// </summary>
	/// <returns></returns>
	FVector2D GetMouseVelocity();
	
	//Check the direction of the thumbstick/mouse in order to change the rotation of the player
	FRotator GetThumbstickAimDirection();
	FRotator GetMouseAimDirection();

	/// <summary>
	/// Updates the player aim.
	/// </summary>
	void UpdatePlayerAim();
	
	/// <summary>
	/// The aim right on x axis input
	/// </summary>
	float aimRightXInput;
	/// <summary>
	/// The aim forward on y axis input
	/// </summary>
	float aimForwardYInput;

	/// <summary>
	/// The main character
	/// </summary>
	ATopDownSurvivalCharacter* mainCharacter;

	/// <summary>
	/// The game mode
	/// </summary>
	ATopDownSurvivalGameMode* gameMode;
};


