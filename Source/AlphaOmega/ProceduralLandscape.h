// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralLandscape.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralLandscape : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the landscape*/
	virtual void GenerateMesh();

public:

	/** Sets the size in width, lenght and height*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector size;

	/** Allocates the number width sections*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 widthSections;

	/** Allocates the number height sections*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 lenghtSections;

	/** Seed used to generate the same height values*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 randomSeed = 1024;

	/** Handles if the figure will be smoothed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	/** Handles if its draw a bottom cap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;

	/** Returns the smooth value of desired index*/
		FVector GetSmoothFromIndex(int32 i, int32 j);

private:

	/** Generates the height for the mesh using randomSeed*/
	void GenerateHeights();

	/** Allocates the number width sections*/
	UPROPERTY()
		TArray<float> heightValues;
};
