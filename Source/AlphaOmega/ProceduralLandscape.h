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

	/** Handles the size of the smooth step*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int smoothStep = 1;

	/** Handles the interpolation mode*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		EInterpolationModes interpolationMode = EInterpolationModes::IM_Linear;

	/** Determines the degree of the curve when we interpolate ease*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float exponential = 1.f;

	/** Determines if the interpolation between points is smoothed or not*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothInterpolation;

	/** Returns the smooth value of desired index*/
		FVector GetSmoothFromIndex(int32 i, int32 j);

private:

	/** Generates the height for the mesh using randomSeed*/
	void GenerateHeights(int smoothStep);

	/** Generates the height for the mesh using randomSeed*/
	void GenerateSmoothTerrain(int smoothStep);

	/** Generates the normal vale smoothed for each vertex based on generated heights*/
	void PrecalculateSmoothNormals();

	/** With the heigth generated we need to calculate the blank positions with interpolation*/
	void InterpolateTerrain(int smoothStep);

	/** Makes the interpolation between A and B using alpha and the interpolation mode desired*/
	float InterpolateVertex(const float a, const float b, const float alpha);

	/** Allocates the number width sections*/
	UPROPERTY()
		TArray<FFloatArray> heightValues;

	/** Allocates the number width sections*/
	UPROPERTY()
		TArray<FVectorArray> smoothValues;
};
