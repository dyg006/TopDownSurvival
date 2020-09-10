// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FMODStudioModule.h"
#include "UObject/Class.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownSurvivalGameMode.generated.h"

class ATopDownSurvivalCharacter;
class ATopDownSurvivalPlayerController;

UENUM()
enum GameStatus
{
	MainMenu     UMETA(DisplayName = "MainMenu"),
	Level1       UMETA(DisplayName =  "Level1"),
};
class AMonsterCharacter;

UCLASS(minimalapi)
class ATopDownSurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATopDownSurvivalGameMode();

	UPROPERTY()
	/// <summary>
	/// The main character controller
	/// </summary>
	ATopDownSurvivalPlayerController* mainCharacterController;

	UPROPERTY()
	/// <summary>
	/// The main character controller
	/// </summary>
	ATopDownSurvivalCharacter* mainCharacter;
	
	UFUNCTION(BlueprintCallable)
	/// <summary>
	/// Gets if the is player dead.
	/// </summary>
	/// <returns></returns>
	bool GetIsPlayerDead() const { return IsPlayerDead; }

	/// <summary>
	/// Sets the is player dead.
	/// </summary>
	/// <param name="isPlayerDead">if set to <c>true</c> [is player dead].</param>
	void SetIsPlayerDead(bool isPlayerDead);

	/// <summary>
	/// Registers the monsters deaths.
	/// </summary>
	/// <param name="monsterCharacter">The monster character.</param>
	void RegisterMonsterDeath(AMonsterCharacter* monsterCharacter);

	UPROPERTY(EditAnywhere, Category = "EnemyClass")
	/// <summary>
	/// The class to spawn
	/// </summary>
	UClass* classToSpawn;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The current wave
	/// </summary>
	int currentWave = 0;

	UPROPERTY(BlueprintReadOnly)
	/// <summary>
	/// The monsters killed
	/// </summary>
	int monstersKilled = 0;

	/// <summary>
	/// Starts the game.
	/// </summary>
	void StartGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The UI pause widget
	/// </summary>
	TSubclassOf<UUserWidget> UIPauseWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The UI lose widget
	/// </summary>
	TSubclassOf<UUserWidget> UILoseWidget;

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The pause widget
	/// </summary>
	UUserWidget* PauseWidget;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	/// <summary>
	/// The lose widget
	/// </summary>
	UUserWidget* LoseWidget;

	/// <summary>
	/// Creates the pause menu.
	/// </summary>
	void CreatePauseMenu();
	
	UFUNCTION(BlueprintCallable, Category = "Widgets")
	/// <summary>
	/// Resumes the game.
	/// </summary>
	void ResumeGame();

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The item picked sound
	/// </summary>
	UFMODEvent* gameoverSound;

	UPROPERTY(EditAnywhere)
	/// <summary>
	/// The item picked sound
	/// </summary>
	UFMODEvent* newWaveSound;

protected:
	void BeginPlay();
	
private:
	/// <summary>
	/// The is player dead
	/// </summary>
	bool IsPlayerDead = false;

	/// <summary>
	/// The current monsters on the game
	/// </summary>
	TArray<AMonsterCharacter*> currentMonsters;
	
	/// <summary>
	/// The spawn points of the monsters
	/// </summary>
	TArray<AActor*> spawnPoints;

	/// <summary>
	/// The timer handle used in order to wait until the start of the wave
	/// </summary>
	FTimerHandle startWaveTimerHandle;

	/// <summary>
	/// The timer handle used in order to spawn enemies of the wave
	/// </summary>
	FTimerHandle waveTimerHandle;
	
	/// <summary>
	/// The timer handle used in order to spawn enemies of the wave
	/// </summary>
	FTimerHandle spawnTimerHandle;

	/// <summary>
	/// The first wave delay
	/// </summary>
	const float firstWaveDelay = 2.f;
	/// <summary>
	/// The next wave delay
	/// </summary>
	const float nextWaveDelay = 5.f;
	/// <summary>
	/// The spawn frequency
	/// </summary>
	const float spawnFrequency = 1.f;

	/// <summary>
	/// The difficulty multiplier
	/// </summary>
	const float difficultyMultiplier = 1.5f;
	
	/// <summary>
	/// The game started
	/// </summary>
	bool gameStarted = false;
	
	/// <summary>
	/// Starts the new wave.
	/// </summary>
	void StartNewWave();
	
	/// <summary>
	/// Spawns a wave.
	/// </summary>
	void SpawnWave();
	
	/// <summary>
	/// Ends the wave.
	/// </summary>
	void EndWave();
	
	/// <summary>
	/// Increases the wave difficulty.
	/// </summary>
	void IncreaseWaveDifficulty();

	/// <summary>
	/// Gets the random spawn point.
	/// </summary>
	/// <param name="spawPoint">The spaw point.</param>
	void GetRandomSpawnPoint(ATargetPoint*& spawPoint);

	/// <summary>
	/// Determines whether [is wave complete].
	/// </summary>
	/// <returns>
	///   <c>true</c> if [is wave complete]; otherwise, <c>false</c>.
	/// </returns>
	bool IsWaveComplete();

	/// <summary>
	/// Creates the lose widget.
	/// </summary>
	void CreateLoseWidget();
	
	/// <summary>
	/// The wave difficulty
	/// </summary>
	int waveDifficulty = 5;
	
	/// <summary>
	/// The aliens spawns remaining
	/// </summary>
	int aliensSpawnsRemaining = 5;
};



