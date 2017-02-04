// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralCube.generated.h"

/**
* This child class of procedural mesh generates a cube
*/
UCLASS()
class ALPHAOMEGA_API AProceduralCube : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the desired figure*/
	virtual void GenerateMesh();

public:

	/** Allocates the desired size of the cube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters", meta = (MakeEditWidget))
	FVector cubeSize;	
	
};
