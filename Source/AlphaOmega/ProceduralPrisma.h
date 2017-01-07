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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float radiusSup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		float radiusInf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector rotationSup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		FVector rotationInf;

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


	virtual void GeneratePrisma(FVector StartPoint, FVector EndPoint, float radiusInf, float radiusSup, FVector rotationInf, FVector rotationSup, int32 &VertexOffset, int32 &TriangleOffset, FVector normal, FProcMeshTangent tangent);

};