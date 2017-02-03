// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralTubeStrip.generated.h"

/**
* Stores the data needed for a point of a strip tube
*/
USTRUCT()
struct FStripPoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		FVector rotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		float radius;

	/** Overloaded constructor*/
	FStripPoint(const FVector& newPosition, const FVector& newRotation, const float& newRadius) {
		position = newPosition;
		rotation = newRotation;
		radius = newRadius;
	}

	/** base constructor*/
	FStripPoint() {
		position = FVector::ZeroVector;
		rotation = FVector::ZeroVector;
		radius = 0.0f;
	}
};

/**
* This child class of procedural mesh generates a piramid
*/
UCLASS()
class ALPHAOMEGA_API AProceduralTubeStrip : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the desired figure*/
	virtual void GenerateMesh();

public:

	/** Allocates the points of the tube strip*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<FStripPoint> stripPoints;

	/** Allocates the points of the tube strip*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<AActor*> sstripPoints;

	/** Allocates the number of faces of each tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 circleSections;

	/** Handles if the figure will be smoothed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	/** Handles if its use an unique texture*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;

	/** Handles if its draw a bottom cap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool addCaps;

	/** Handles if its draw a bottom cap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool isClosedStrip;
};
