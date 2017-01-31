// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralSphere.h"




void AProceduralSphere::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(0, 0, 0);

	GenerateSphere(center, radius, VertexOffset, TriangleOffset, normal, tangent);
}




void AProceduralSphere::GenerateSphere(FVector center, float radius, int32 &VertexOffset, int32 &TriangleOffset, FVector normal, FProcMeshTangent tangent) {

	const float angleBetweenAltitude = 360.0f / (float)circleSections;
	const float VMapPerAltitude = 1.0f / (float)circleSections;
	const float angleBetweenLatitude = (180.0f / (float)heightSections);
	const float VMapPerLatitude = 1.0f / (float)heightSections;

	FVector pInitStart = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f) + center;

	for (int32 i = 0; i < circleSections; ++i)
	{
		float angleAltitude = (float)i * angleBetweenAltitude;
		float nextAngleAltitude = (float)(i + 1) * angleBetweenAltitude;

		// Set up the vertices
		
		for (int32 j = 0; j < heightSections / 2; ++j) {

			float angleLatitude = (float)j * angleBetweenLatitude;
			float nextAngleLatitude = (float)(j + 1) * angleBetweenLatitude;

			FVector p0 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, angleLatitude, angleAltitude));
			FVector p1 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, angleLatitude, nextAngleAltitude));
			FVector p2 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, nextAngleLatitude, nextAngleAltitude));
			FVector p3 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, nextAngleLatitude, angleAltitude));

			FVector currentNormal = (((p3 + p2 + p1 + p0) / 4) - center).GetSafeNormal();
			BuildQuad(p3, p2, p1, p0, VertexOffset, TriangleOffset, currentNormal, tangent);

			if (smoothNormals)
			{

				// To smooth normals we give the vertices different values than the polygon they belong to.
				// GPUs know how to interpolate between those.
				// I do this here as an average between normals of two adjacent polygons
				normals[normals.Num() - 4] = (p3 - center).GetSafeNormal();
				normals[normals.Num() - 3] = (p2 - center).GetSafeNormal();
				normals[normals.Num() - 2] = (p1 - center).GetSafeNormal();
				normals[normals.Num() - 1] = (p0 - center).GetSafeNormal();


			}

			p0 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, -angleLatitude, nextAngleAltitude));
			p1 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, -angleLatitude, angleAltitude));
			p2 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, -nextAngleLatitude, angleAltitude));
			p3 = RotatePointAroundPivot(pInitStart, center, FVector(.0f, -nextAngleLatitude, nextAngleAltitude));

			currentNormal = (((p3 + p2 + p1 + p0) / 4) - center).GetSafeNormal();
			BuildQuad(p3, p2, p1, p0, VertexOffset, TriangleOffset, currentNormal, tangent);

			if (smoothNormals)
			{
				
				// To smooth normals we give the vertices different values than the polygon they belong to.
				// GPUs know how to interpolate between those.
				// I do this here as an average between normals of two adjacent polygons
				normals[normals.Num() - 4] = (p3 - center).GetSafeNormal();
				normals[normals.Num() - 3] = (p2 - center).GetSafeNormal();
				normals[normals.Num() - 2] = (p1 - center).GetSafeNormal();
				normals[normals.Num() - 1] = (p0 - center).GetSafeNormal();
				

			}

			if (useUniqueTexture)
			{
				// UVs.  Note that Unreal UV origin (0,0) is top left
				UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (VMapPerAltitude * (i + 1)), 0.5f + (VMapPerLatitude * j));
				UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (VMapPerAltitude * i), 0.5f + (VMapPerLatitude * j));
				UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (VMapPerAltitude * i), 0.5f + (VMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 4] = FVector2D(1.0f - (VMapPerAltitude * (i + 1)), 0.5f + (VMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 5] = FVector2D(1.0f - (VMapPerAltitude * i),			0.5f - (VMapPerLatitude * j));
				UV0s[UV0s.Num() - 6] = FVector2D(1.0f - (VMapPerAltitude * (i + 1)), 0.5f - (VMapPerLatitude * j));
				UV0s[UV0s.Num() - 7] = FVector2D(1.0f - (VMapPerAltitude * (i + 1)), 0.5f - (VMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 8] = FVector2D(1.0f - (VMapPerAltitude * i), 0.5f - (VMapPerLatitude * (j + 1)));
			}




		}
	}
}