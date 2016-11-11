// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "AlphaOmegaGameMode.generated.h"

// Enum to store the curent state of gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState {

	EPlaying,
	EGameOver,
	EWon,
	EUnknow
};

UCLASS(minimalapi)
class AAlphaOmegaGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AAlphaOmegaGameMode();

	virtual void Tick(float DeltaTime) override;

	/** Returns power needed to win - needed for the HUD*/
	UFUNCTION(BlueprintPure, Category = "Power")
		float GetPowerToWin() const;

	virtual void BeginPlay() override;

	/** Returns the current playing state*/
	UFUNCTION(BlueprintPure, Category = "Power")
		EBatteryPlayState GetCurrentState() const;

	/** Sets the new playing state*/
	void SetCurrentState(EBatteryPlayState NewState);

protected:
	/** The rate at witch the character loses power*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintPortected = "true"))
		float DecayRate;

	/** Power needed to win the game*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintPortected = "true"))
		float PowerToWin;

	/** The widget class to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintPortected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The intance of the HUD*/
	UPROPERTY()
		class UUserWidget* CurrentWidget;

private:
	/** Keeps track of the current playing state*/
	EBatteryPlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	/** Handle any function calls that rely upon changing the playing state of our game*/
	void HandleNewState(EBatteryPlayState NewState);
};






