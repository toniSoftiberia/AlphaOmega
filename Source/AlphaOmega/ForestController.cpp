// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ForestController.h"


// Sets default values
AForestController::AForestController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	extension = 10000;
	NumberOfBigTrees = 1;
}

// Called when the game starts or when spawned
void AForestController::BeginPlay()
{
	Super::BeginPlay();

	const int32 mushroomsNeeded = CountMushrooms();
	/*
	// -- Value Sets -- //
	usedPlaces.SetNum(mushroomsNeeded);*/

	GenerateForest();
}

// Called every frame
void AForestController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// Spawn Point Gens
FVector AForestController::GenerateCoord(const int32 level, const FVector epicenter) {

	UE_LOG(LogClass, Log, TEXT("epicenter %s"), *epicenter.ToString());

	FVector passBack;

	int32 attempts = 0;

	do {
		if (level == 0) {
			passBack.Set(FMath::RandRange(-extension, extension),
				FMath::RandRange(-extension, extension),
				0.f);
		}
		else {

			ASpawnable* XXX = Cast<ASpawnable>(MushroomContainer[level - 1]->GetDefaultObject());
			//float influence = MushroomContainer[level];
			UE_LOG(LogClass, Log, TEXT("influencexxx %f"), XXX->GetInfluenceRadius());
			UE_LOG(LogClass, Log, TEXT("influencexxx %f"), XXX->GetMinDistanceToSpawnSubs());
			UE_LOG(LogClass, Log, TEXT("influencexxx %f"), XXX->GetMinDistanceToSpawnSame());
			UE_LOG(LogClass, Log, TEXT("influencexxx %d"), XXX->GetNumberOfChilds());
			//UE_LOG(LogClass, Log, TEXT("influencexxx %s"), XXX->GetSpawnedPosition().ToString());

			float influence = MushroomContainer[level - 1]->GetDefaultObject<ASpawnable>()->GetInfluenceRadius();
			UE_LOG(LogClass, Log, TEXT("influence %f"), influence);

			float epiXpos = epicenter.X + influence;
			float epiXneg = epicenter.X - influence;
			float epiYpos = epicenter.Y + influence;
			float epiYneg = epicenter.Y - influence;

			UE_LOG(LogClass, Log, TEXT("bounds %f, %f, %f, %f"), epiXpos, epiXneg, epiYpos, epiYneg);

			float randX = FMath::RandRange(epiXneg, epiXpos);
			float randY = FMath::RandRange(epiYneg, epiYpos);

			UE_LOG(LogClass, Log, TEXT("clamped %f, %f"), randX, randY);

			passBack.Set(FMath::RandRange(-extension, extension),
				FMath::RandRange(-extension, extension),
				0.f);

			UE_LOG(LogClass, Log, TEXT("passBack %s"), *passBack.ToString());
			passBack.Set(randX,
				randY,
				0.f);
			UE_LOG(LogClass, Log, TEXT("passBack %s"), *passBack.ToString());
		}
		++attempts;
	} while (CanBeUsed(level, passBack) && attempts < 20);

	if (attempts == 10) {
		return FVector::ZeroVector;
	}

	return passBack;

}

bool AForestController::CanBeUsed(const int32 level, const FVector& placeCheck) {

	bool isUsed = false;

	for (int32 i = 0; i < usedPlaces.Num() && !isUsed; ++i) {

		// Get the distance between nodes
		float distance = (usedPlaces[i]->GetSpawnedPosition() - placeCheck).Size();

		// Check if they are of the same type
		if (usedPlaces[i]->GetNodeSize() == MushroomContainer[level]->GetDefaultObject<ASpawnable>()->GetNodeSize()) {

			isUsed = distance < usedPlaces[i]->GetMinDistanceToSpawnSame();
			if (level == 0)
				UE_LOG(LogClass, Log, TEXT("distance %f : level %d"), distance, level);
		}
		else {
			isUsed = distance < usedPlaces[i]->GetMinDistanceToSpawnSubs() + MushroomContainer[level]->GetDefaultObject<ASpawnable>()->GetMinDistanceToSpawnSubs();
		}

	}

	return isUsed;
}

// Calculate the total number of mushrooms of the forest
const int32 AForestController::CountMushrooms() {

	int32 totalOfMushrooms = 0;

	if (MushroomContainer.Num() > 0) {

		totalOfMushrooms = NumberOfBigTrees;
		for (int i = 1; i < MushroomContainer.Num(); ++i) {

			int32 count = MushroomContainer[i - 1]->GetDefaultObject<ASpawnable>()->GetNumberOfChilds();
			totalOfMushrooms *= count;
		}
	}

	UE_LOG(LogClass, Log, TEXT("totalOfMushrooms %d"), totalOfMushrooms);

	return totalOfMushrooms;
}


// Spawns a blueprint
ASpawnable* AForestController::SpawnObject(const FVector& placeToSpawn, const int32 level) {

	// Check if the world exist
	const UWorld* world = GetWorld();

	if (world) {

		// Create spawn params
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		// Rotate it randomly in Z
		FRotator SpawnRotation = FRotator::ZeroRotator;
		SpawnRotation.Yaw = FMath::FRand() * 360.0f;

		// Spawn the object
		int32 num = FMath::RandRange(0, 2);

		ASpawnable* newObjectSpawned = GetWorld()->SpawnActor<ASpawnable>(MushroomContainer[level], placeToSpawn, SpawnRotation, spawnParams);

		if (newObjectSpawned)
			newObjectSpawned->SetSpawnedPosition(placeToSpawn);

		return (ASpawnable*)newObjectSpawned;
	}

	return nullptr;
}

// Checks if the distance with all the other mushrooms is OK
bool AForestController::CheckDistance(const FVector& placeToSpawn) {

	return true;
}



/// Spawn Point Gens
bool AForestController::GenerateForest() {

	return GenerateLevel(0, NumberOfBigTrees, FVector::ZeroVector);
}

/// Spawn Point Gens
bool AForestController::GenerateLevel(const int32 level, const int32 NumberOfNodes, const FVector centerOfLevel) {

	bool abort = false;

	for (int32 i = 0; i < NumberOfNodes && !abort; i++) {

		// Holds the next position where a tree will be spawned
		FVector nextPlace = GenerateCoord(level, centerOfLevel);

		if (nextPlace != FVector::ZeroVector) {

			ASpawnable* spawnedObject = SpawnObject(nextPlace, level);

			// Add the used place to the control array
			usedPlaces.Add(spawnedObject);

			if (level + 1 < MushroomContainer.Num()) {
				UE_LOG(LogClass, Log, TEXT("GetSpawnedPosition %s"), *spawnedObject->GetSpawnedPosition().ToString());
				GenerateLevel(level + 1, spawnedObject->GetNumberOfChilds(), spawnedObject->GetSpawnedPosition());
			}
		}
		else {
			abort = true;
			UE_LOG(LogClass, Log, TEXT("Forest Aborted"));
		}
	}

	return true;
}