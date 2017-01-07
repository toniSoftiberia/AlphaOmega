// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralPiramid.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralPiramid : public AProceduralMesh
{
	GENERATED_BODY()

	virtual void GenerateMesh();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 circleSections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool invertedSmoothNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool addCaps;
	
	
	
};