// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTube.h"



void AProceduralTube::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);
	

	// Make a cylinder section
	const float AngleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float VMapPerQuad = 1.0f / (float)circleSections;
	FVector Offset = FVector(0, 0, height);

	FVector pInit = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f) + (Offset /2);
	
	for (int32 QuadIndex = 0; QuadIndex < circleSections; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(Angle) * radius, FMath::Sin(Angle) * radius, 0.f)) + (Offset / 2);
		FVector p1 = (FVector(FMath::Cos(NextAngle) * radius, FMath::Sin(NextAngle) * radius, 0.f)) + (Offset / 2);
		FVector p2 = p1 - Offset;
		FVector p3 = p0 - Offset;

		BuildQuad(p0, p1, p2, p3, VertexOffset, TriangleOffset, normal, tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 1.0f);
			UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 1.0f);
			UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 4] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 0.0f);
		}

		// Calculate face normal
		FVector NormalCurrent = FVector::CrossProduct(vertices[vertices.Num()-4] - vertices[vertices.Num() - 2], vertices[vertices.Num() - 3] - vertices[vertices.Num() - 2]).GetSafeNormal();

		if (smoothNormals || invertedSmoothNormals)
		{

			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			float NextNextAngle = (float)(QuadIndex + 2) * AngleBetweenQuads;
			FVector p4 = FVector(FMath::Cos(NextNextAngle) * radius, FMath::Sin(NextNextAngle) * radius, 0.f);

			float PreviousAngle = (float)(QuadIndex - 1) * AngleBetweenQuads;
			FVector pMinus1 = FVector(FMath::Cos(PreviousAngle) * radius, FMath::Sin(PreviousAngle) * radius, 0.f);
			
			FVector AverageNormalRight, AverageNormalLeft;

			if (!invertedSmoothNormals) {
				// p1 to p4 to p2
				FVector NormalNext = FVector::CrossProduct(p1 - p2, p4 - p2).GetSafeNormal();
				AverageNormalRight = (NormalCurrent + NormalNext) / 2;
				AverageNormalRight = AverageNormalRight.GetSafeNormal();

				// p0 to p3 to pMinus1
				FVector NormalPrevious = FVector::CrossProduct(p0 - pMinus1, p3 - pMinus1).GetSafeNormal();
				AverageNormalLeft = (NormalCurrent + NormalPrevious) / 2;
				AverageNormalLeft = AverageNormalLeft.GetSafeNormal();
			}
			else {
				// p3 to p4 to p1
				FVector NormalNext = FVector::CrossProduct(p3 - p1, p4 - p1).GetSafeNormal();
				AverageNormalRight = (NormalCurrent + NormalNext) / 2;
				AverageNormalRight = AverageNormalRight.GetSafeNormal();

				// p2 to p0 to pMinus1
				FVector NormalPrevious = FVector::CrossProduct(p2 - pMinus1, p0 - pMinus1).GetSafeNormal();
				AverageNormalLeft = (NormalCurrent + NormalPrevious) / 2;
				AverageNormalLeft = AverageNormalLeft.GetSafeNormal();
			}

			normals[normals.Num() - 4] = AverageNormalLeft;
			normals[normals.Num() - 3] = AverageNormalRight;
			normals[normals.Num() - 2] = AverageNormalRight;
			normals[normals.Num() - 1] = AverageNormalLeft;
		}
		else
		{
			// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
			normals[normals.Num() - 4] = normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;
		}

		// -------------------------------------------------------
		// Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
		// A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
		if (QuadIndex != 0 && addCaps)
		{
			// Bottom cap
			BuildTriangle(
				pInit, p1, p0,
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

			// Top cap
			BuildTriangle(
				pInit - Offset, p0 - Offset, p1 - Offset,
				VertexOffset,
				TriangleOffset,
				normal,
				tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));

			NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();
			normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;
			
		}
	}
}

