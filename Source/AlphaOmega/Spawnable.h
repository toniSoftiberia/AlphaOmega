// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Spawnable.generated.h"

UENUM(BlueprintType)
enum class ENodeSize : uint8 {

	ESizeXL,
	ESizeL,
	ESizeM,
	ESizeS,
	ESizeXS
};

UCLASS()
class ALPHAOMEGA_API ASpawnable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Return the minimum distance to spawn*/
	FORCEINLINE float GetMinDistanceToSpawnSubs() const { return MinDistanceToSpawnSubs; }

	/** Return the minimum distance to spawn*/
	FORCEINLINE float GetMinDistanceToSpawnSame() const { return MinDistanceToSpawnSame; }

	/** Return the maximum distance to spawn*/
	FORCEINLINE float GetInfluenceRadius() const { return InfluenceRadius; }

	/** Return the number of childs to spawn*/
	FORCEINLINE int32 GetNumberOfChilds() const { return NumberOfChilds; }

	/** Return the number of childs to spawn*/
	FORCEINLINE FVector GetSpawnedPosition() const { return SpawnedPosition; }

	/** Allow other classes to safely change the spawned position*/
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetSpawnedPosition(FVector NewSpawnedPosition);


	/** Returns the current playing state*/
	UFUNCTION(BlueprintPure, Category = "Power")
		ENodeSize GetNodeSize() const;

protected:

	/** Minimum distance to spawn objects more little than this*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn", Meta = (BlueprintPortected = "true"))
		float MinDistanceToSpawnSubs;

	/** Minimum distance to spawn objects to spawn object like this*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn", Meta = (BlueprintPortected = "true"))
		float MinDistanceToSpawnSame;

	/** Maximum distance to spawn child objects*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn", Meta = (BlueprintPortected = "true"))
		float InfluenceRadius;

	/** Number of childs to spawn*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn", Meta = (BlueprintPortected = "true", ClampMin = "2", ClampMax = "10"))
		int32 NumberOfChilds;

	/** Place where Object has been spawned*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn", Meta = (BlueprintPortected = "true", ClampMin = "2", ClampMax = "10"))
		FVector SpawnedPosition;

	/** Place where Object has been spawned*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn", Meta = (BlueprintPortected = "true", ClampMin = "0", ClampMax = "4"))
		ENodeSize NodeSize;


};
