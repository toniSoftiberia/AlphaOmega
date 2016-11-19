// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralTriangle.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralTriangle : public AProceduralMesh
{
	GENERATED_BODY()
	
	virtual void GenerateMesh();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector vertexA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector vertexB;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector vertexC;
	


};
