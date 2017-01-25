// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralPrisma.h"


void AProceduralPrisma::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);

	GeneratePrisma(startPoint, endPoint, startRotation, endRotation, startRadius, endRadius, VertexOffset, TriangleOffset, normal, tangent);
}

void AProceduralPrisma::GeneratePrisma(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 &VertexOffset, int32 &TriangleOffset, FVector normal, FProcMeshTangent tangent) {
/*
	// Get prisma orientation and flip it to get its perpendicular
	FVector orientation = endPoint - startPoint;

	
	UE_LOG(LogClass, Log, TEXT("orientation           : %s"), *orientation.ToString());
	//orientation.Rotation().Add(0.f, 0.f, 90.f).Euler();

	
	FVector orthogonal = FVector(1, 3, 0);

	if (orientation.Z != 0)
		orthogonal.Z = -(orientation.X * orthogonal.X + orientation.Y * orthogonal.Y) / orientation.Z;
	else
		orthogonal.Z = orientation.X * orthogonal.X + orientation.Y * orthogonal.Y;

	orientation = orthogonal;

	UE_LOG(LogClass, Log, TEXT("orientation.Z %f"), orientation.Z);
	UE_LOG(LogClass, Log, TEXT("orientation.Y %f"), orientation.Y);
	UE_LOG(LogClass, Log, TEXT("orientation.X %f"), orientation.X);
	UE_LOG(LogClass, Log, TEXT("orthogonal.Z %f"), orthogonal.Z);
	UE_LOG(LogClass, Log, TEXT("orthogonal.Y %f"), orthogonal.Y);
	UE_LOG(LogClass, Log, TEXT("orthogonal.X %f"), orthogonal.X);

	orientation.Normalize();
	UE_LOG(LogClass, Log, TEXT("orientation Normalized: %s"), *orientation.ToString());
	
	FVector orientationFinal = FVector::CrossProduct(orientation, FVector(0, 0, 1));
	UE_LOG(LogClass, Log, TEXT("orientation Cross pro1: %s"), *orientationFinal.ToString());
	
	if (orientationFinal == FVector::ZeroVector) {
		orientationFinal = FVector::CrossProduct(orientation, FVector(0, 1, 0));
		UE_LOG(LogClass, Log, TEXT("orientation Cross pro2: %s"), *orientationFinal.ToString());
		if (orientationFinal == FVector::ZeroVector) {
			orientationFinal = FVector::CrossProduct(orientation, FVector(1, 0, 1));
			UE_LOG(LogClass, Log, TEXT("orientation Cross pro3: %s"), *orientationFinal.ToString());
		}
	}*/


	// Get prisma orientation and flip it to get its perpendicular
	FVector orientation = endPoint - startPoint;

	orientation = orientation.Rotation().Add(90.f, 0.f, 0.f).Euler();

	// Make a cylinder section
	const float AngleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float VMapPerQuad = 1.0f / (float)circleSections;
	
	FVector pInitStart = FVector(FMath::Cos(0) * startRadius, FMath::Sin(0) * startRadius, 0.f) + startPoint;
	FVector pInitEnd = FVector(FMath::Cos(0) * endRadius, FMath::Sin(0) * endRadius, 0.f) + endPoint;
	pInitStart = RotatePointAroundPivot(pInitStart, startPoint, startRotation + orientation);
	pInitEnd = RotatePointAroundPivot(pInitEnd, endPoint, endRotation + orientation);

	for (int32 QuadIndex = 0; QuadIndex < circleSections; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(Angle) * endRadius, FMath::Sin(Angle) * endRadius, 0.f)) + endPoint;
		FVector p1 = (FVector(FMath::Cos(NextAngle) * endRadius, FMath::Sin(NextAngle) * endRadius, 0.f)) + endPoint;
		FVector p2 = (FVector(FMath::Cos(NextAngle) * startRadius, FMath::Sin(NextAngle) * startRadius, 0.f)) + startPoint;
		FVector p3 = (FVector(FMath::Cos(Angle) * startRadius, FMath::Sin(Angle) * startRadius, 0.f)) + startPoint;


		p0 = RotatePointAroundPivot(p0, endPoint, endRotation + orientation);
		p1 = RotatePointAroundPivot(p1, endPoint, endRotation + orientation);
		p2 = RotatePointAroundPivot(p2, startPoint, startRotation + orientation);
		p3 = RotatePointAroundPivot(p3, startPoint, startRotation + orientation);

		BuildQuad(p1, p0, p3, p2, VertexOffset, TriangleOffset, normal, tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 1.0f);
			UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 1.0f);
			UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 4] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 0.0f);
		}

		// Calculate face normal
		FVector NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 4] - vertices[vertices.Num() - 2], vertices[vertices.Num() - 3] - vertices[vertices.Num() - 2]).GetSafeNormal();
		
		if (smoothNormals || invertedSmoothNormals)
		{
			
			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			float NextNextAngle = (float)(QuadIndex + 2) * AngleBetweenQuads;
			FVector p4 = FVector(FMath::Cos(NextNextAngle) * startRadius, FMath::Sin(NextNextAngle) * startRadius, 0.f);

			float PreviousAngle = (float)(QuadIndex - 1) * AngleBetweenQuads;
			FVector pMinus1 = FVector(FMath::Cos(PreviousAngle) * startRadius, FMath::Sin(PreviousAngle) * startRadius, 0.f);

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
			
			// Start cap
			BuildTriangle(
				pInitStart, p2, p3,
				//pInitStart, p3, p2,
				VertexOffset,
				TriangleOffset,
				normal,
				tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));

			NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();
			normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;
			
			// Tangents (perpendicular to the surface)
			FVector SurfaceTangent = p0 - p1;
			SurfaceTangent = SurfaceTangent.GetSafeNormal();
			
			
			// End cap
			BuildTriangle(
				//p1, p0, pInitSup,
				p0,  p1, pInitEnd, 
				VertexOffset,
				TriangleOffset,
				normal,
				tangent);

			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));

			NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();
			normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;
			
		}
	}
}