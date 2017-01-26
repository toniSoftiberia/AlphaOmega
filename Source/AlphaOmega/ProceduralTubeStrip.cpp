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

	FVector vectorA = LinePoints[1] - LinePoints[0];
	UE_LOG(LogClass, Log, TEXT("vectorA %s"), *vectorA.ToString());
	FVector vectorB = LinePoints[1] - LinePoints[2];
	UE_LOG(LogClass, Log, TEXT("vectorB %s"), *vectorB.ToString());
	FVector vectorC = vectorA - vectorB;
	UE_LOG(LogClass, Log, TEXT("vectorC %s"), *vectorC.ToString());
	FVector RotationAngle = vectorC.Rotation().Add(-90.f, 00.f, 0.f).Euler();
	FVector RotationAngle2 = vectorC.Rotation().Add(00.f, 90.f, 0.f).Euler();

	vectorA.Normalize();
	vectorB.Normalize();

	if (vectorA.X == -1)
		vectorA.X *= -1;
	if (vectorA.Y == -1)
		vectorA.Y *= -1;
	if (vectorA.Z == -1)
		vectorA.Z *= -1;

	if (vectorB.X == -1)
		vectorB.X *= -1;
	if (vectorB.Y == -1)
		vectorB.Y *= -1;
	if (vectorB.Z == -1)
		vectorB.Z *= -1;

	UE_LOG(LogClass, Log, TEXT("RotationAngle %s"), *RotationAngle.ToString());
	UE_LOG(LogClass, Log, TEXT("RotationAngle2 %s"), *RotationAngle2.ToString());

	//vectorC.Normalize();


	GeneratePrisma(LinePoints[0], LinePoints[1], RotationAngle, RotationAngle, radius2, radius2, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(LinePoints[1], LinePoints[2], RotationAngle2, RotationAngle2, radius2, radius2, VertexOffset, TriangleOffset, normal, tangent);
	/*
	GeneratePrisma(LinePoints[0], LinePoints[1], FVector::ZeroVector, -RotationAngle, radius2, radius2, VertexOffset, TriangleOffset, normal, tangent);
	GeneratePrisma(LinePoints[1], LinePoints[2], RotationAngle, FVector::ZeroVector, radius2, radius2, VertexOffset, TriangleOffset, normal, tangent);*/
	/*
	for (int i = 0; i < LinePoints.Num() - 1; ++i) {
		GeneratePrisma(LinePoints[i], LinePoints[i + 1], radius1, radius2, FVector::ZeroVector, rotationMiddle, VertexOffset, TriangleOffset, normal, tangent);

		GeneratePrisma(LinePoints[i], LinePoints[i+1], radius2, radius2, FVector::ZeroVector, vectorA + vectorB, VertexOffset, TriangleOffset, normal, tangent);
	}*/

}

