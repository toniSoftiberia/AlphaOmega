// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"

void AProceduralTubeStrip::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(1,1,1);

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

	GenerateTube(LinePoints[0], LinePoints[1], RotationD, RotationA, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, VertexOffset, TriangleOffset, tangent);
	GenerateTube(LinePoints[1], LinePoints[2], RotationA, RotationB, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, VertexOffset, TriangleOffset, tangent);
	GenerateTube(LinePoints[2], LinePoints[3], RotationB, RotationC, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, VertexOffset, TriangleOffset, tangent);
	GenerateTube(LinePoints[3], LinePoints[0], RotationC, RotationD, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, VertexOffset, TriangleOffset, tangent);
}

