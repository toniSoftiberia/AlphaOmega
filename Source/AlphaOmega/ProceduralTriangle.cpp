// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTriangle.h"


void AProceduralTriangle::GenerateMesh() {

	FVector normal = FVector::CrossProduct(vertexC - vertexA, vertexB - vertexA).GetSafeNormal();

	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildTriangle(
		vertexA, vertexB, vertexC,
		normal,
		tangent);
}

