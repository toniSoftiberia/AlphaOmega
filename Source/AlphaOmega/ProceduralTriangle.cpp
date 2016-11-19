// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTriangle.h"


void AProceduralTriangle::GenerateMesh() {

	int32 vertexOffset = 0;

	int32 triangleOffset = 0;

	FVector normal = FVector::ZeroVector;

	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildTriangle(
		vertexA, vertexB, vertexC,
		vertexOffset,
		triangleOffset,
		normal,
		tangent);
}

