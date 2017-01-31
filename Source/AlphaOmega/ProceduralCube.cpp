// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralCube.h"



void AProceduralCube::GenerateMesh() {

	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildCube(cubeSize, VertexOffset, TriangleOffset, tangent);
}

