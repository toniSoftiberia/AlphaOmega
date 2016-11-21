// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"

void AProceduralTubeStrip::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	GenerateTube(FVector::ZeroVector, FVector(0, 0, height), VertexOffset, TriangleOffset, normal, tangent);
	GenerateTube(FVector(0, 100, height), FVector(0, 100, height), VertexOffset, TriangleOffset, normal, tangent);
}

