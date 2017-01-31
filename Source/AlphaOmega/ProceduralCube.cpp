// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralCube.h"



void AProceduralCube::GenerateMesh() {

	int32 vertexOffset = 0;
	int32 triangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildCube(cubeSize, vertexOffset, triangleOffset, tangent);
}

