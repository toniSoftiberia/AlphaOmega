// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralQuad.generated.h"


/**
* This child class of procedural mesh generates a quad
*/
UCLASS()
class ALPHAOMEGA_API AProceduralQuad : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the desired figure*/
	virtual void GenerateMesh();

public:

	/** Allocates bottom left vertex of the quad*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector bottomLeft;

	/** Allocates bottom right vertex of the quad*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector bottomRight;

	/** Allocates top right vertex of the quad*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector topRight;

	/** Allocates top left vertex of the quad*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector topLeft;
	
	
	
};
