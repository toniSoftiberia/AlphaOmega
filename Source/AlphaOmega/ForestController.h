// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spawnable.h"
#include "ForestController.generated.h"

UCLASS()
class ALPHAOMEGA_API AForestController : public AActor
{
	GENERATED_BODY()

		/// Public information -- Constructis and engine events
public:

	// Sets default values for this actor's properties
	AForestController();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/// Private Information -- Spawning Methods
private:

	/// Generate a coord to use
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		FVector GenerateCoord(const int32 level, const FVector epicenter);

	/// Checks id the value has lveen used before, by testung an array
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		bool CanBeUsed(const int32 level, const FVector& placeCheck);

	/* Calculate the total number of mushrooms of the forest */
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		const int32 CountMushrooms();

	/** Spawns an spawnable object*/
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		ASpawnable* SpawnObject(const FVector& placeToSpawn, const int32 level);

	/** Checks if two spawnable are in the right distance */
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		bool CheckDistance(const FVector& placeToSpawn);

	/** Generates the forest looking for all details needed*/
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		bool GenerateForest();

	/** Generates the forest looking for all details needed*/
	UFUNCTION(BlueprintCallable, Category = "Forest Controller")
		bool GenerateLevel(const int32 level, const int32 NumberOfNodes, const FVector centerOfLevel);

	/// Private information -- Data types and containers
private:

	/// An Array to hold all tree positions
	TArray<ASpawnable*> usedPlaces;


	/// Protected information -- Spawned Objects
protected:

	/// First possible object
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Settings")
		TArray<TSubclassOf<ASpawnable>> MushroomContainer;

public:

	/// Third Possible Object
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int32 extension;

	/** Minimum distance to spawn objects*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn Settings", Meta = (BlueprintPortected = "true", ClampMin = "2", ClampMax = "10"))
		int32 NumberOfBigTrees;
};
