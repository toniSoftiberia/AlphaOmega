// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"
#include "ProceduralPrisma.h"

void AProceduralTubeStrip::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1,1,1);

	float radius = 10.0f;

	FVector vectorA = LinePoints[1] - LinePoints[0];
	FVector vectorB = LinePoints[1] - LinePoints[2];
	FVector RotationA = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	vectorA = LinePoints[2] - LinePoints[1];
	vectorB = LinePoints[2] - LinePoints[3];
	FVector RotationB = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	vectorA = LinePoints[3] - LinePoints[2];
	vectorB = LinePoints[3] - LinePoints[0];
	FVector RotationC = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	vectorA = LinePoints[0] - LinePoints[3];
	vectorB = LinePoints[0] - LinePoints[1];
	FVector RotationD = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	GeneratePrisma(LinePoints[0], LinePoints[1], RotationD, RotationA, radius, radius, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(LinePoints[1], LinePoints[2], RotationA, RotationB, radius, radius, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(LinePoints[2], LinePoints[3], RotationB, RotationC, radius, radius, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(LinePoints[3], LinePoints[0], RotationC, RotationD, radius, radius, VertexOffset, TriangleOffset, normal, tangent);

}

