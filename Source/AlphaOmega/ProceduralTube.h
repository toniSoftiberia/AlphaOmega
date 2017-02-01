// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralTube.generated.h"

/**
* This child class of procedural mesh generates a piramid
*/
UCLASS()
class ALPHAOMEGA_API AProceduralTube : public AProceduralMesh
{
	GENERATED_BODY()

	/** Sets up the values to generate the desired figure*/
	virtual void GenerateMesh();

public:

	/** Allocates the start point of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector startPoint;

	/** Allocates the end point of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector endPoint;

	/** Allocates the start rotation of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector startRotation;

	/** Allocates the end rotation of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector endRotation;

	/** Allocates the start radius of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float startRadius;

	/** Allocates the end radius of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float endRadius;

	/** Allocates the number of faces of the tube*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 circleSections;

	/** Handles if the figure will be smoothed*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	/** Handles if its use an unique texture*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;

	/** Handles if its draw a bottom cap*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool addCaps;	
};
