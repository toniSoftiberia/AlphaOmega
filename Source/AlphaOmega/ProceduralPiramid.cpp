// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralPiramid.h"

void AProceduralPiramid::GenerateMesh() {


	// Now we create 6x faces, 4 vertices each
	int32 vertexOffset = 0;
	int32 triangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildPiramid(height,  radius, circleSections, smoothNormals, useUniqueTexture, addBottomCap, vertexOffset, triangleOffset, tangent);
}



