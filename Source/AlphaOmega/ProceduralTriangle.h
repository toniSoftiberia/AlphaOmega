// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralTriangle.generated.h"

/**
* This child class of procedural mesh generates a triangle
*/
UCLASS()
class ALPHAOMEGA_API AProceduralTriangle : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the desired figure*/
	virtual void GenerateMesh();

public:

	/** Allocates first vertex of the triangle*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector vertexA;

	/** Allocates second vertex of the triangle*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector vertexB;

	/** Allocates thirth vertex of the triangle*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector vertexC;
};
