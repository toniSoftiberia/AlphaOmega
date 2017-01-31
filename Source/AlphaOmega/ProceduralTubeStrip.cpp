// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"

void AProceduralTubeStrip::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 vertexOffset = 0;
	int32 triangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(1,1,1);

	FVector vectorA = LinePoints[1] - LinePoints[0];
	FVector vectorB = LinePoints[1] - LinePoints[2];
	FVector rotationA = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	vectorA = LinePoints[2] - LinePoints[1];
	vectorB = LinePoints[2] - LinePoints[3];
	FVector rotationB = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	vectorA = LinePoints[3] - LinePoints[2];
	vectorB = LinePoints[3] - LinePoints[0];
	FVector rotationC = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	vectorA = LinePoints[0] - LinePoints[3];
	vectorB = LinePoints[0] - LinePoints[1];
	FVector rotationD = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

	BuildTube(LinePoints[0], LinePoints[1], rotationD, rotationA, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, vertexOffset, triangleOffset, tangent);
	BuildTube(LinePoints[1], LinePoints[2], rotationA, rotationB, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, vertexOffset, triangleOffset, tangent);
	BuildTube(LinePoints[2], LinePoints[3], rotationB, rotationC, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, vertexOffset, triangleOffset, tangent);
	BuildTube(LinePoints[3], LinePoints[0], rotationC, rotationD, startRadius, startRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, vertexOffset, triangleOffset, tangent);
}

