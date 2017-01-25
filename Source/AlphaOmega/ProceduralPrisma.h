// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMesh.h"
#include "ProceduralPrisma.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralPrisma : public AProceduralMesh
{
	GENERATED_BODY()

		virtual void GenerateMesh();

public:

	// Prima attributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector startPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector endPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector startRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector endRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float startRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float endRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		int32 circleSections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool addCaps;

	// Common attributes

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool smoothNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool invertedSmoothNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		bool useUniqueTexture;


	virtual void GeneratePrisma(FVector StartPoint, FVector EndPoint, FVector rotationInf, FVector rotationSup, float radiusInf, float radiusSup, int32 &VertexOffset, int32 &TriangleOffset, FVector normal, FProcMeshTangent tangent);

};