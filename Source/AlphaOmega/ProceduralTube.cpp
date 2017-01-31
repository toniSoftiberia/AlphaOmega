// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTube.h"



void AProceduralTube::GenerateMesh() {

	int32 vertexOffset = 0;
	int32 triangleOffset = 0;
	FProcMeshTangent tangent = FProcMeshTangent(0, 0, 0);

	FVector orientation = startPoint - endRotation;

	BuildTube(startPoint, endPoint, startRotation, endRotation, startRadius, endRadius, circleSections, smoothNormals, useUniqueTexture, addCaps, vertexOffset, triangleOffset, tangent);
}

