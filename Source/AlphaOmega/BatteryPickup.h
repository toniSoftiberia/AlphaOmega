// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABatteryPickup();

	/** Override WasCollected function - use Implementation because it's a Blueprint Native Event*/
	void WasCollected_Implementation() override;

	/** Public way to acces the battery power's level*/
	float GetPower();

protected:
	/** Set the amount of power the battery gives to the player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
		float BatteryPower;
};
