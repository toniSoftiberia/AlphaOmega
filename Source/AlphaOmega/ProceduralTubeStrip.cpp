// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"
#include "ProceduralPrisma.h"

void AProceduralTubeStrip::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	float radius1 = 50.0f;
	float radius2 = 20.0f;
	float radius3 = 30.0f;
	FVector rotationMiddle = FVector(0, 15, 0);

	GeneratePrisma(FVector::ZeroVector, FVector(0, 0, height/2), radius1, radius2, FVector::ZeroVector, rotationMiddle, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(FVector(0, 0, height / 2), FVector(0, 0, height), radius2, radius3, rotationMiddle, FVector::ZeroVector, VertexOffset, TriangleOffset, normal, tangent);
}

