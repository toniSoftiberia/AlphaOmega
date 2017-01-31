// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTube.h"



void AProceduralTube::GenerateMesh() {

	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(0, 0, 0);

	FVector orientation = startPoint - endRotation;

	//startRotation += orientation;
	//endRotation += orientation;

	GenerateTube(startPoint, endPoint, startRotation, endRotation, startRadius, endRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, VertexOffset, TriangleOffset, tangent);
}

