// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralPiramid.h"




void AProceduralPiramid::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);


	// Make a cylinder section
	const float AngleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float VMapPerQuad = 1.0f / (float)circleSections;
	FVector Offset = FVector(0, 0, height);

	FVector pInit = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f);

	for (int32 QuadIndex = 0; QuadIndex < circleSections; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(Angle) * radius, FMath::Sin(Angle) * radius, 0.f));
		FVector p1 = (FVector(FMath::Cos(NextAngle) * radius, FMath::Sin(NextAngle) * radius, 0.f));

		// Calculate face normal
		FVector NormalCurrent = FVector::CrossProduct(p0 - Offset, p1 - Offset).GetSafeNormal();

		BuildTriangle( p1, p0, Offset, VertexOffset, TriangleOffset, NormalCurrent, tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (VMapPerQuad ), 1.0f);
			UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 0.0f);
		}


		if (smoothNormals)
		{

			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			normals[normals.Num() - 3] = (p1).GetSafeNormal();
			normals[normals.Num() - 2] = (p0).GetSafeNormal();
			//normals[normals.Num() - 1] = ((p0 - p1) - Offset).GetSafeNormal();
			normals[normals.Num() - 1] = FVector::UpVector;


		}

		// -------------------------------------------------------
		// Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
		// A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
		if (QuadIndex != 0 && addCaps)
		{
			// Cap
			BuildTriangle(
				pInit, p0, p1,
				VertexOffset,
				TriangleOffset,
				normal,
				tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));

			NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();
			normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;

			// Tangents (perpendicular to the surface)
			FVector SurfaceTangent = p0 - p1;
			SurfaceTangent = SurfaceTangent.GetSafeNormal();
			
		}
	}
}



