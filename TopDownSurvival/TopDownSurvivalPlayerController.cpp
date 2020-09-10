// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownSurvivalPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TopDownSurvivalCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ATopDownSurvivalPlayerController::ATopDownSurvivalPlayerController()
{
	//bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}


void ATopDownSurvivalPlayerController::BeginPlay()
{
	currentPawn = this->GetPawn();
	mainCharacter = Cast<ATopDownSurvivalCharacter>(currentPawn);
	gameMode = Cast<ATopDownSurvivalGameMode>(GetWorld()->GetAuthGameMode());
	//RootComponent->SetUsingAbsoluteRotation(false);
}

void ATopDownSurvivalPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

}

void ATopDownSurvivalPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATopDownSurvivalPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATopDownSurvivalPlayerController::MoveRight);
	InputComponent->BindAxis("AimForward", this, &ATopDownSurvivalPlayerController::AimForward);
	InputComponent->BindAxis("AimRight", this, &ATopDownSurvivalPlayerController::AimRight);

	InputComponent->BindAction("FireWeapon", IE_Pressed, this, &ATopDownSurvivalPlayerController::StartFiringWeapon);
	InputComponent->BindAction("FireWeapon", IE_Released, this, &ATopDownSurvivalPlayerController::StopFiringWeapon);

	InputComponent->BindAction("Pause", IE_Pressed, this, &ATopDownSurvivalPlayerController::Pause).bExecuteWhenPaused = true;
	
	InputComponent->BindAction("IsGamepadActive", IE_Pressed, this, &ATopDownSurvivalPlayerController::AnyKeyPressed);

}


void ATopDownSurvivalPlayerController::MoveForward(float value)
{
	if (value != 0.0f && currentPawn != nullptr)
	{
		if (gameMode != nullptr && gameMode->GetIsPlayerDead()) {
			return;
		}
		
		const FVector Direction = FVector(1.f, 0.f, 0.f);
		
		currentPawn->AddMovementInput(Direction, value);
	}
}

void ATopDownSurvivalPlayerController::MoveRight(float value)
{
	if (value != 0.0f && currentPawn != nullptr)
	{
		if (gameMode != nullptr && gameMode->GetIsPlayerDead()) {
			return;
		}
		
		const FVector Direction = FVector(0.f, 1.f, 0.f);
		
		// add movement in that direction
		currentPawn->AddMovementInput(Direction, value);

	}
}

void ATopDownSurvivalPlayerController::UpdatePlayerAim()
{
	if (isGamepadActive)
	{
		//Check if the input value is enough to move the aim
		if (IsThumbstickAboveDeadzone())
		{
			currentPawn->SetActorRotation(GetThumbstickAimDirection());
			SetControlRotation(GetThumbstickAimDirection());
		}
	}
	else
	{
		if (IsMouseAboveDeadzone())
		{
			currentPawn->SetActorRotation(GetMouseAimDirection());
			SetControlRotation(GetMouseAimDirection());
		}
	}
}

void ATopDownSurvivalPlayerController::AimForward(float value)
{
	if (gameMode != nullptr && gameMode->GetIsPlayerDead()) {
		return;
	}
	if (isGamepadActive)
	{
		
		aimForwardYInput = value;
		//Check if the input value is enough to move the aim
		if(IsThumbstickAboveDeadzone())
		{
			currentPawn->SetActorRotation(GetThumbstickAimDirection());
			SetControlRotation(GetThumbstickAimDirection());
		}
	}// else	THE MOUSE AIM IS DONE WITH CURSOR 
	//{
	//	
	//	if(IsMouseAboveDeadzone())
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("AIMFORWARD CON RATÓN ACTIVO!! X: %lf Y: %lf Z: %lf"), GetMouseAimDirection().Roll, GetMouseAimDirection().Pitch, GetMouseAimDirection().Yaw));
	//		currentPawn->SetActorRotation(GetMouseAimDirection());
	//		SetControlRotation(GetMouseAimDirection());
	//	}
	//}
}

void ATopDownSurvivalPlayerController::AimRight(float value)
{
	if (gameMode != nullptr && gameMode->GetIsPlayerDead()) {
		return;
	}
	if (isGamepadActive)
	{
		aimRightXInput = value;
		//Check if the input value is enough to move the aim
		if (IsThumbstickAboveDeadzone())
		{
			currentPawn->SetActorRotation(GetThumbstickAimDirection());
			SetControlRotation(GetThumbstickAimDirection());
		}
	}
	/*else    THE MOUSE AIM IS DONE WITH CURSOR 
	{
		if (IsMouseAboveDeadzone())
		{
			currentPawn->SetActorRotation(GetMouseAimDirection());
			SetControlRotation(GetMouseAimDirection());
		}
	}*/
}

void ATopDownSurvivalPlayerController::StartFiringWeapon()
{
	if (gameMode != nullptr && !gameMode->GetIsPlayerDead()) {
		if (mainCharacter != nullptr)
		{
			mainCharacter->StartFiringWeapon();
		}
	}
}

void ATopDownSurvivalPlayerController::StopFiringWeapon()
{
	if (gameMode != nullptr && !gameMode->GetIsPlayerDead()) {		
		if (mainCharacter != nullptr)
		{
			mainCharacter->StopFiringWeapon();
		}
	}
}

void ATopDownSurvivalPlayerController::AnyKeyPressed(FKey key)
{
	
	if(UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("MainMenu"))
	{
		gameMode->StartGame();
	} 
	if (key.IsGamepadKey())
	{
		isGamepadActive = true;
	}
	else
	{
		isGamepadActive = false;
	}
}

void ATopDownSurvivalPlayerController::Pause()
{
	if (gameMode != nullptr)
	{
		if (UGameplayStatics::GetCurrentLevelName(GetWorld()) != TEXT("MainMenu"))
		{
			gameMode->CreatePauseMenu();
		}
	}
}


bool ATopDownSurvivalPlayerController::IsThumbstickAboveDeadzone()
{
	FVector2D currentInput = FVector2D (aimRightXInput, aimForwardYInput);
	return currentInput.Size() > thumbstickDeadzone;
}

bool ATopDownSurvivalPlayerController::IsMouseAboveDeadzone()
{
	FVector2D currentInput = GetMouseVelocity();
	
	return currentInput.Size() > mouseDeadzone;
}

FVector2D ATopDownSurvivalPlayerController::GetMouseVelocity()
{
	FVector2D mouseInput;
	GetInputMouseDelta(mouseInput.X, mouseInput.Y);
	return mouseInput / UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
}

FRotator ATopDownSurvivalPlayerController::GetThumbstickAimDirection()
{
	FRotator thumbstickRotation = FRotator( 0.f, atan2(aimRightXInput, aimForwardYInput) * 180 / PI, 0.f);
	return thumbstickRotation;
}

FRotator ATopDownSurvivalPlayerController::GetMouseAimDirection()
{
	FVector2D mouseVelocity = GetMouseVelocity();
	FRotator mouseRotation = FRotator( 0.f, atan2(mouseVelocity.X, mouseVelocity.Y) * 180 / PI, 0.f);
	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Green, FString::Printf(TEXT("MOUSE ROTATION!!atan: %lf"), atan2(mouseVelocity.X, mouseVelocity.Y)* 180 / 3.14));
	return FMath::RInterpTo(GetControlRotation(), mouseRotation, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), mouseSensitivity);
}

void ATopDownSurvivalPlayerController::UpdateCharacterRotation(const FVector cursorPosition)
{
	if (gameMode != nullptr && gameMode->GetIsPlayerDead()) {
		return;
	}
	if (!isGamepadActive) {
		//Target - Start
		FVector playerView = cursorPosition - mainCharacter->GetActorLocation();
		playerView.Z = 0.f;
		FRotator playerNewRotation = FRotationMatrix::MakeFromX(playerView).Rotator();
		mainCharacter->SetActorRotation(playerNewRotation);
		SetControlRotation(playerNewRotation);
	}
}


