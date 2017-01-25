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
	float radius2 = 10.0f;
	float radius3 = 30.0f;
	FVector rotationMiddle = FVector(0, 15, 0);

	FVector vectorA = LinePoints[0] - LinePoints[1];
	FVector vectorB = LinePoints[1] - LinePoints[2];
	FVector vectorC = vectorA - vectorB;
	//vectorC.Normalize();
	FVector RotationAngle = vectorC.Rotation().Add(90.f, 0.f, 0.f).Euler();


	GeneratePrisma(LinePoints[0], LinePoints[1], FVector::ZeroVector, -RotationAngle, radius2, radius2, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(LinePoints[1], LinePoints[2], RotationAngle, FVector::ZeroVector, radius2, radius2, VertexOffset, TriangleOffset, normal, tangent);

	for (int i = 0; i < LinePoints.Num() - 1; ++i) {
		//GeneratePrisma(LinePoints[i], LinePoints[i + 1], radius1, radius2, FVector::ZeroVector, rotationMiddle, VertexOffset, TriangleOffset, normal, tangent);

		//GeneratePrisma(LinePoints[i], LinePoints[i+1], radius2, radius2, FVector::ZeroVector, vectorA + vectorB, VertexOffset, TriangleOffset, normal, tangent);
	}

}

