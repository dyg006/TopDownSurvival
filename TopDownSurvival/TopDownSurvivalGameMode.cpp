// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownSurvivalGameMode.h"
#include "FMODBlueprintStatics.h"
#include "AI/MonsterCharacter.h"
#include "TopDownSurvivalPlayerController.h"
#include "TopDownSurvivalCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ATopDownSurvivalGameMode::ATopDownSurvivalGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ATopDownSurvivalPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacter/BP_MarineCharacter"));
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) != TEXT("MainMenu")) //Check if the map is main menu so we do not spawn the main character
	{
		if (PlayerPawnBPClass.Class != NULL)
		{
			DefaultPawnClass = PlayerPawnBPClass.Class;
		}
	}
}

void ATopDownSurvivalGameMode::BeginPlay()
{
	
	Super::BeginPlay();

	mainCharacter = Cast<ATopDownSurvivalCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	mainCharacterController = Cast<ATopDownSurvivalPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), spawnPoints);
	if (spawnPoints.Num() == 0) //We are not in a level
	{
		return;
	}
	GetWorldTimerManager().SetTimer(startWaveTimerHandle, this, &ATopDownSurvivalGameMode::StartNewWave, firstWaveDelay, false);
}

void ATopDownSurvivalGameMode::StartGame()
{
	if(mainCharacterController != nullptr)
	{
		FInputModeGameOnly inputGameOnly;
		mainCharacterController->SetInputMode(inputGameOnly);
		mainCharacterController->bShowMouseCursor = false;
		//Load level 1
		UGameplayStatics::OpenLevel(GetWorld(),TEXT("Mars"));
		
	}
}

void ATopDownSurvivalGameMode::SetIsPlayerDead(bool isPlayerDead)
{
	IsPlayerDead = isPlayerDead;
	if (IsPlayerDead)
	{
		CreateLoseWidget();
	}
}

void ATopDownSurvivalGameMode::RegisterMonsterDeath(AMonsterCharacter* monsterCharacter)
{
	++monstersKilled;
	currentMonsters.Remove(monsterCharacter);
	if(IsWaveComplete())
	{
		EndWave();
	}
}

void ATopDownSurvivalGameMode::StartNewWave()
{
	//if it is the first wave we clear the first timer, if not we clear the rest of them
	if(!gameStarted)
	{
		gameStarted = true;
		GetWorldTimerManager().ClearTimer(startWaveTimerHandle);
	} else
	{
		GetWorldTimerManager().ClearTimer(waveTimerHandle);
	}
	++currentWave;
	IncreaseWaveDifficulty();
	GetWorldTimerManager().SetTimer(spawnTimerHandle, this, &ATopDownSurvivalGameMode::SpawnWave, spawnFrequency, true);
}

void ATopDownSurvivalGameMode::IncreaseWaveDifficulty()
{
	//Increase the number of enemies that spawns per round
	waveDifficulty = FMath::RoundToInt(difficultyMultiplier * waveDifficulty);
	aliensSpawnsRemaining = waveDifficulty;
}

void ATopDownSurvivalGameMode::SpawnWave()
{
	ATargetPoint* spawnPoint;
	GetRandomSpawnPoint(spawnPoint);

	if(spawnPoint != nullptr)
	{
		AMonsterCharacter* monsterCreated;
		//we spawn the next monster and add him to the array
		monsterCreated = GetWorld()->SpawnActor<AMonsterCharacter>(classToSpawn ,spawnPoint->GetActorLocation(), spawnPoint->GetActorRotation());
		if(monsterCreated != nullptr)
		{
			currentMonsters.Add(monsterCreated);
			--aliensSpawnsRemaining;
			if(aliensSpawnsRemaining == 0)
			{
				GetWorldTimerManager().ClearTimer(spawnTimerHandle);
			}
		}		
	}
}


void ATopDownSurvivalGameMode::GetRandomSpawnPoint(ATargetPoint*& spawnPoint)
{
	if(spawnPoints.Num() == 0)
	{
		return;
	}
	//Get a random spawn point from the spawn points array
	spawnPoint = Cast<ATargetPoint>(spawnPoints[FMath::RandRange(0, spawnPoints.Num() - 1)]);
}

bool ATopDownSurvivalGameMode::IsWaveComplete()
{
	if(currentMonsters.Num() == 0 && aliensSpawnsRemaining == 0)
	{
		return true;
	} else
	{
		return false;
	}
}

void ATopDownSurvivalGameMode::EndWave()
{
	if (newWaveSound != nullptr && mainCharacter != nullptr) {
		UFMODBlueprintStatics::PlayEventAttached(newWaveSound, mainCharacter->GetRootComponent(), "", mainCharacter->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
	}
	GetWorldTimerManager().SetTimer(waveTimerHandle, this, &ATopDownSurvivalGameMode::StartNewWave, nextWaveDelay, false);
}

void ATopDownSurvivalGameMode::CreateLoseWidget()
{
	if (UILoseWidget != nullptr) {
		LoseWidget = CreateWidget<UUserWidget>(mainCharacterController, UILoseWidget);
		LoseWidget->AddToViewport();

		mainCharacterController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
		mainCharacterController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		mainCharacterController->SetInputMode(mainCharacterController->inputModeGameAndUI);

		LoseWidget->SetKeyboardFocus();
		mainCharacterController->bShowMouseCursor = true;

		if (gameoverSound != nullptr && mainCharacter != nullptr) {
			UFMODBlueprintStatics::PlayEventAttached(gameoverSound, mainCharacter->GetRootComponent(), "", mainCharacter->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
		}
		//mainCharacter->ResetCharacterParams();
	}
}

void ATopDownSurvivalGameMode::CreatePauseMenu() {

	if (UGameplayStatics::IsGamePaused(this)) {
		if (UIPauseWidget != nullptr) {
			ResumeGame();
		}
	}
	else {
		if (UIPauseWidget != nullptr) {
			PauseWidget = CreateWidget<UUserWidget>(mainCharacterController, UIPauseWidget);
			PauseWidget->AddToViewport();

			mainCharacterController->inputModeGameAndUI.SetHideCursorDuringCapture(true);
			mainCharacterController->inputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			mainCharacterController->SetInputMode(mainCharacterController->inputModeGameAndUI);

			PauseWidget->SetKeyboardFocus();
			mainCharacterController->bShowMouseCursor = true;

			UGameplayStatics::SetGamePaused(this, true);
			UE_LOG(LogTemp, Warning, TEXT("Activating pause widget"));
		}
	}
}

void ATopDownSurvivalGameMode::ResumeGame()
{
	if (PauseWidget)
	{
		mainCharacterController->inputModeGameOnly.SetConsumeCaptureMouseDown(false);
		mainCharacterController->SetInputMode(mainCharacterController->inputModeGameOnly);

		mainCharacterController->bShowMouseCursor = false;
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;

		UGameplayStatics::SetGamePaused(this, false);
		UE_LOG(LogTemp, Warning, TEXT("Deactivating pause widget"));
	}
}