// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralQuad.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralQuad : public AProceduralMesh
{
	GENERATED_BODY()

	virtual void GenerateMesh();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector bottomLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector bottomRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector topRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector topLeft;
	
	
	
};
