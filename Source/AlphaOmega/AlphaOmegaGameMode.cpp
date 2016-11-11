// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "AlphaOmega.h"
#include "AlphaOmegaGameMode.h"
#include "AlphaOmegaCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"

AAlphaOmegaGameMode::AAlphaOmegaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Base decay rate
	DecayRate = 0.01f;
}

void AAlphaOmegaGameMode::BeginPlay() {

	Super::BeginPlay();

	// find all spawn volume Actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors) {
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor) {

			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	// Set the score to beat
	AAlphaOmegaCharacter* MyCharacter = Cast<AAlphaOmegaCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter) {
		PowerToWin = MyCharacter->GetInitialPower() * 1.25f;
	}

	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr) {

			CurrentWidget->AddToViewport();
		}
	}
}

void AAlphaOmegaGameMode::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	// Check that we are using our Battery collector character
	AAlphaOmegaCharacter* MyCharacter = Cast<AAlphaOmegaCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	// if the character's power is positive
	if (MyCharacter) {

		// If our power is greater than needed to win, set the game's state to won
		if (MyCharacter->GetCurrentPower() > PowerToWin) {
			SetCurrentState(EBatteryPlayState::EWon);
		}

		// If the character's power is positive
		else if (MyCharacter->GetCurrentPower() > 0) {
			// decrease the character's power using the decay rate'
			MyCharacter->UpdatePower(-DeltaTime*DecayRate*(MyCharacter->GetInitialPower()));
		}

		else {
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}

float AAlphaOmegaGameMode::GetPowerToWin() const {

	return PowerToWin;
}

EBatteryPlayState AAlphaOmegaGameMode::GetCurrentState() const {

	return CurrentState;
}

void AAlphaOmegaGameMode::SetCurrentState(EBatteryPlayState NewState) {

	// Set current state
	CurrentState = NewState;

	// Handle the new current state 
	HandleNewState(CurrentState);
}


void AAlphaOmegaGameMode::HandleNewState(EBatteryPlayState NewState) {

	switch (NewState) {
		// If the game is playing
	case EBatteryPlayState::EPlaying: {
		// Spawns volumes active
		for (ASpawnVolume* Volume : SpawnVolumeActors) {
			Volume->SetSpawningActive(true);
		}
	}
									  break;
									  // If we've wone the game
	case EBatteryPlayState::EWon: {
		// Spawns volumes inactive
		for (ASpawnVolume* Volume : SpawnVolumeActors) {
			Volume->SetSpawningActive(false);
		}
	}
								  break;
								  // If we've los the game
	case EBatteryPlayState::EGameOver: {
		// Spawns volumes inactive
		for (ASpawnVolume* Volume : SpawnVolumeActors) {
			Volume->SetSpawningActive(false);
		}

		// Block Player input
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController) {

			PlayerController->SetCinematicMode(true, false, false, true, true);
		}

		// Ragdoll the character
		ACharacter* MyCharacter = (UGameplayStatics::GetPlayerCharacter(this, 0));
		if (MyCharacter) {

			MyCharacter->GetMesh()->SetSimulatePhysics(true);
			MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
		}

	}
									   break;
									   // Unknow/default state
	default:
	case EBatteryPlayState::EUnknow: {
		// do nothing
	}
									 break;

	}
}
