// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralCube.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralCube : public AProceduralMesh
{
	GENERATED_BODY()

	virtual void GenerateMesh();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	FVector cubeSize;
	
	
	
};
