// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralCube.h"



void AProceduralCube::GenerateMesh() {

	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildCube(cubeSize, tangent);
}

