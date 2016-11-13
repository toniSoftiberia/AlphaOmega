// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "AlphaOmegaCharacter.h"
#include "BatteryPickup.h"


// Sets default values
ABatteryPickup::ABatteryPickup() {

	GetMesh()->SetSimulatePhysics(true);

	// The base power level of the battery
	BatteryPower = 150.f;
}


void ABatteryPickup::WasCollected_Implementation() {

	// Use the base pickup behavior 
	Super::WasCollected_Implementation();

	// Destroy the battery
	Destroy();

	// Set new state
	BatteryUpdateCollecting(false);

}


// report the power level of the battery
float ABatteryPickup::GetPower() {

	return BatteryPower;
}





// Sets the new state to is collecting
void ABatteryPickup::BatteryUpdateCollecting(bool NewState) {

	// Get player
	AAlphaOmegaCharacter* MyCharacter = Cast<AAlphaOmegaCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (MyCharacter) {
		// Set new state
		MyCharacter->UpdateCollecting(false);
	}
}