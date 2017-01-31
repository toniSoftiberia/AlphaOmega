// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralSphere.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralSphere : public AProceduralMesh
{
	GENERATED_BODY()

	virtual void GenerateMesh();

public:

	// Sphere attributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector center;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 circleSections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 heightSections;

	// Common attributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool invertedSmoothNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;

};
