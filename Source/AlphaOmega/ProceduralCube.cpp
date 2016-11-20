// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralCube.h"



void AProceduralCube::GenerateMesh() {

	// Calculate a half offset so we get correct center of object
	float OffsetX = cubeSize.X / 2.0f;
	float OffsetY = cubeSize.Y / 2.0f;
	float OffsetZ = cubeSize.Z / 2.0f;

	// Define the 8 corners of the cube
	FVector p0 = FVector(OffsetX, OffsetY, -OffsetZ);
	FVector p1 = FVector(OffsetX, -OffsetY, -OffsetZ);
	FVector p2 = FVector(OffsetX, -OffsetY, OffsetZ);
	FVector p3 = FVector(OffsetX, OffsetY, OffsetZ);
	FVector p4 = FVector(-OffsetX, OffsetY, -OffsetZ);
	FVector p5 = FVector(-OffsetX, -OffsetY, -OffsetZ);
	FVector p6 = FVector(-OffsetX, -OffsetY, OffsetZ);
	FVector p7 = FVector(-OffsetX, OffsetY, OffsetZ);

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent();


	// Front (+X) face: 0-1-2-3
	//normal = FVector(1, 0, 0);
	//tangent = FProcMeshTangent(1, 1, 1);
	BuildQuad(p0, p1, p2, p3, VertexOffset, TriangleOffset, normal, tangent);

	// Back (-X) face: 5-4-7-6
	//normal = FVector(-1, 0, 0);
	//tangent = FProcMeshTangent(1, 1, 1);
	BuildQuad(p5, p4, p7, p6, VertexOffset, TriangleOffset, normal, tangent);

	// Left (-Y) face: 1-5-6-2
	//normal = FVector(0, -1, 0);
	//tangent = FProcMeshTangent(1, 1, 1);
	BuildQuad(p1, p5, p6, p2, VertexOffset, TriangleOffset, normal, tangent);

	// Right (+Y) face: 4-0-3-7
	//normal = FVector(0, 1, 0);
	//tangent = FProcMeshTangent(1, 1, 1);
	BuildQuad(p4, p0, p3, p7, VertexOffset, TriangleOffset, normal, tangent);

	// Top (+Z) face: 6-7-3-2
	//normal = FVector(0, 0, 1);
	//tangent = FProcMeshTangent(1, 1, 1);
	BuildQuad(p6, p7, p3, p2, VertexOffset, TriangleOffset, normal, tangent);

	// Bottom (-Z) face: 1-0-4-5
	//normal = FVector(0, 0, -1);
	//tangent = FProcMeshTangent(1, 1, 1);
	BuildQuad(p1, p0, p4, p5, VertexOffset, TriangleOffset, normal, tangent);
}

