// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralSphere.h"




void AProceduralSphere::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 vertexOffset = 0;
	int32 triangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(0, 0, 0);

	BuildSphere(center, radius, circleSections, heightSections, smoothNormals, useUniqueTexture, vertexOffset, triangleOffset, tangent);
}



