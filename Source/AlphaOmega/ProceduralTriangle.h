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

	/** Allocates the points of the tube strip*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters", meta = (MakeEditWidget))
		TArray<FTransform> verticesLocation;
};
