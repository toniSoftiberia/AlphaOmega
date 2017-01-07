// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralPrisma.h"
#include "ProceduralTubeStrip.generated.h"

/**
 * 
 */
UCLASS()
class ALPHAOMEGA_API AProceduralTubeStrip : public AProceduralPrisma
{
	GENERATED_BODY()

	virtual void GenerateMesh();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<FVector> LinePoints;

	//virtual void GenerateTubeStrip(FVector StartPoint, FVector EndPoint, int32 &VertexOffset, int32 &TriangleOffset, FVector normal, FProcMeshTangent tangent);

	/*
	void GenerateMesh();
	void GenerateCylinder(TArray<FRuntimeMeshVertexSimple>& Vertices, TArray<int32>& Triangles, FVector StartPoint, FVector EndPoint, float InWidth, int32 InCrossSectionCount, int32& VertexIndex, int32& TriangleIndex, bool bInSmoothNormals = true);
	FBox GetBounds();

	FVector RotatePointAroundPivot(FVector InPoint, FVector InPivot, FVector InAngles);
	void PreCacheCrossSection();

	int32 LastCachedCrossSectionCount;
	UPROPERTY(Transient)
		TArray<FVector> CachedCrossSectionPoints;

	// Mesh buffers
	void SetupMeshBuffers();
	bool bHaveBuffersBeenInitialized = false;
	TArray<FRuntimeMeshVertexSimple> Vertices;
	TArray<int32> Triangles;
	*/
	
};
