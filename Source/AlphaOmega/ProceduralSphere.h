// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralSphere.generated.h"

/**
* This child class of procedural mesh generates a sphere
*/
UCLASS()
class ALPHAOMEGA_API AProceduralSphere : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the desired figure*/
	virtual void GenerateMesh();

public:

	/** Use geodesic method to build the sphere*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool bGeodesic;

	/** Allocates the center point of the sphere*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector center;

	/** Allocates the desired radius of the sphere*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float radius;

	/** Indicates the depth of diviosions of the triangles of the sphere*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters", meta = (EditCondition = "bGeodesic"))
		int32 depth;

	/** Allocates the number of horizontal faces of the sphere*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters", meta = (EditCondition = "!bGeodesic"))
		int32 circleSections;

	/** Allocates the number of vertical faces of the sphere*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters", meta = (EditCondition = "!bGeodesic"))
		int32 heightSections;

	/** Handles if the figure will be smoothed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	/** Handles if its draw a bottom cap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;
};
