// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralPiramid.h"

void AProceduralPiramid::GenerateMesh() {


	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildPiramid(height,  radius, circleSections, smoothNormals, useUniqueTexture, addBottomCap, VertexOffset, TriangleOffset, tangent);
}



