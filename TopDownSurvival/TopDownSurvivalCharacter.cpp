// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownSurvivalCharacter.h"
#include "TopDownSurvivalPlayerController.h"
#include "FMODBlueprintStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/Rifle.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AI/MonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"

ATopDownSurvivalCharacter::ATopDownSurvivalCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthPlayerComponent"));
	healthComponent->Init(100);

	firingComponentSound = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FireComponentSound"));
	firingComponentSound->SetupAttachment(RootComponent);

	hurtComponentSound = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("HurtComponentSound"));
	hurtComponentSound->SetupAttachment(RootComponent);

	heartbeatSound = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("HeartbeatSoundComponent"));
	heartbeatSound->SetupAttachment(RootComponent);
	
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATopDownSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	gameMode = Cast<ATopDownSurvivalGameMode>(GetWorld()->GetAuthGameMode());
	
	//Initially starts on player
	weaponEquipped = Cast<ARifle>(UGameplayStatics::GetActorOfClass(GetWorld(), ARifle::StaticClass()));
	//weaponEquipped = Cast<ARifle>(GetWorld()->SpawnActor(ARifle::StaticClass()));
	if (weaponEquipped != nullptr) {
		weaponEquipped->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponPoint");
	}
	playerController = Cast<ATopDownSurvivalPlayerController>(GetController());
	UpdateHeartbeatSound();

	collectionInstance = GetWorld()->GetParameterCollectionInstance(parameterCollection);

}


void ATopDownSurvivalCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr && playerController != nullptr)
	{
		if (!playerController->GetIsGamepadActive()) {
			CursorToWorld->SetHiddenInGame(false);
			FHitResult TraceHitResult;
			playerController->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		} else
		{
			CursorToWorld->SetHiddenInGame(true);
		}
		playerController->UpdateCharacterRotation(CursorToWorld->GetComponentLocation());

	}
}


void ATopDownSurvivalCharacter::StartFiringWeapon()
{
	if(weaponEquipped != nullptr)
	{
		weaponEquipped->ActivateParticles();
		//Spawn the sound
		if (weaponEquipped->firingSound != nullptr) {
			firingComponentSound = UFMODBlueprintStatics::PlayEventAttached(weaponEquipped->firingSound, GetMesh(), "", GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, false);
		}
		WeaponTrace();
		GetWorldTimerManager().SetTimer(timerHandle, this, &ATopDownSurvivalCharacter::WeaponTrace, weaponEquipped->GetWeaponFireRate(), true);
	}
}

void ATopDownSurvivalCharacter::StopFiringWeapon()
{
	if (weaponEquipped != nullptr)
	{
		weaponEquipped->DeactivateParticles();

		//stop the firing sound
		if(firingComponentSound != nullptr)
		{
			firingComponentSound->Stop();
		}
		GetWorldTimerManager().ClearTimer(timerHandle);
	}
}

void ATopDownSurvivalCharacter::WeaponTrace()
{
	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(
		hit,
		GetWeaponTraceStartLocation(),
		GetWeaponTraceEndLocation(),
		ECollisionChannel::ECC_Visibility
	);
	if(hit.bBlockingHit)
	{
		AMonsterCharacter* monsterCharacter = Cast<AMonsterCharacter>(hit.Actor);
		if(monsterCharacter != nullptr)
		{
			weaponEquipped->CauseDamageTo(monsterCharacter, hit);
		}	
		weaponEquipped->SpawnParticlesCollision(hit.Location);
	}
}

FVector ATopDownSurvivalCharacter::GetWeaponTraceStartLocation()
{
	return weaponEquipped->GetWeaponSocketLocation(TEXT("MuzzleFlash"));
}

FVector ATopDownSurvivalCharacter::GetWeaponTraceEndLocation()
{
	return GetWeaponTraceStartLocation() + GetControlRotation().Vector() * weaponEquipped->GetWeaponRange();
}

float ATopDownSurvivalCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// DamageType example and properties
	UDamageType const* const DamageTypeCDO = DamageEvent.DamageTypeClass ? DamageEvent.DamageTypeClass->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	DamageTypeCDO->DamageImpulse;
	DamageTypeCDO->bCausedByWorld;

	if (ActualDamage != 0.f)
	{
		this->healthStatusEvent.Broadcast(this->healthComponent->health);
		healthComponent->Damage(ActualDamage);

		if (playerHurt != nullptr)
		{
			GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(playerHurt);
		}
		// if dead destroy
		if (!healthComponent->alive) {

			//Activate the death animation of the player (it is used as a delay too)		
			if (gameMode != nullptr)
			{
				if (!gameMode->GetIsPlayerDead()) {
					gameMode->SetIsPlayerDead(true);
					StopFiringWeapon();
				}
			}

			ActualDamage = 0.f;
			if (heartbeatSound != nullptr) {
				// set the parameter that control the beat sound to 1 again so the sound is not activated again when the player is dead
				heartbeatSound->SetParameter("Health", 1);
			}
			DeactivateDamageShader();
		}
		else {
			if (hurtComponentSound != nullptr && !hurtComponentSound->IsPlaying()) {
				hurtComponentSound->Play();
			}
			UpdateHeartbeatSound();
			if(healthComponent->GetHealthPercentage() <= lowHealthPercentage)
			{
				ActivateDamageShader();
			}
		}
	}

	return ActualDamage;

}

void ATopDownSurvivalCharacter::UpdateHeartbeatSound() {
	if (heartbeatSound != nullptr) {
		//	// set the parameter that control the beat sound normalizing the health value
		heartbeatSound->SetParameter("Health", healthComponent->health / healthComponent->defaultHealth);
	}
}

void ATopDownSurvivalCharacter::ResetCharacterParams() {
	healthComponent->health = healthComponent->defaultHealth;

	UpdateHeartbeatSound();
}

void ATopDownSurvivalCharacter::ActivateDamageShader() {	
	if (collectionInstance != nullptr)
	{
		collectionInstance->SetScalarParameterValue("Blood", 1.f);
	}
}

void ATopDownSurvivalCharacter::DeactivateDamageShader() {
	if(collectionInstance != nullptr)
	{
		collectionInstance->SetScalarParameterValue("Blood", 0.f);
	}
}