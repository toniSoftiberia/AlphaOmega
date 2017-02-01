// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralQuad.h"



void AProceduralQuad::GenerateMesh() {

	FVector normal = FVector::CrossProduct(topRight - topLeft, bottomLeft - topLeft).GetSafeNormal();

	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	BuildQuad(
		bottomLeft, bottomRight, topRight, topLeft,
		normal,
		tangent);
}
