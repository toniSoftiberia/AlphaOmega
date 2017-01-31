// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralPrisma.h"


void AProceduralPrisma::GenerateMesh() {

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	//FProcMeshTangent tangent = FProcMeshTangent(1, 1, 1);
	FProcMeshTangent tangent = FProcMeshTangent(0,0,0);

	GeneratePrisma(startPoint, endPoint, startRotation, endRotation, startRadius, endRadius, VertexOffset, TriangleOffset, normal, tangent);
}

void AProceduralPrisma::GeneratePrisma(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 &VertexOffset, int32 &TriangleOffset, FVector normal, FProcMeshTangent tangent) {

	// Get prisma orientation and flip it to get its perpendicular
	FVector orientation = FVector::ZeroVector;

	if (endRotation == FVector::ZeroVector || startRotation == FVector::ZeroVector) {
		orientation = endPoint - startPoint;
		startRotation = orientation;
		endRotation = orientation;
	}

	// Make a cylinder section
	const float AngleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float VMapPerQuad = 1.0f / (float)circleSections;
	
	FVector pInitStart = FVector(FMath::Cos(0) * startRadius, FMath::Sin(0) * startRadius, 0.f) + startPoint;
	FVector pInitEnd = FVector(FMath::Cos(0) * endRadius, FMath::Sin(0) * endRadius, 0.f) + endPoint;
	pInitStart = RotatePointAroundPivot(pInitStart, startPoint, (startRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());
	pInitEnd = RotatePointAroundPivot(pInitEnd, endPoint, (endRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());

	for (int32 QuadIndex = 0; QuadIndex < circleSections; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(NextAngle) * endRadius, FMath::Sin(NextAngle) * endRadius, 0.f)) + endPoint;
		FVector p1 = (FVector(FMath::Cos(Angle) * endRadius, FMath::Sin(Angle) * endRadius, 0.f)) + endPoint;
		FVector p2 = (FVector(FMath::Cos(NextAngle) * startRadius, FMath::Sin(NextAngle) * startRadius, 0.f)) + startPoint;
		FVector p3 = (FVector(FMath::Cos(Angle) * startRadius, FMath::Sin(Angle) * startRadius, 0.f)) + startPoint;

		FVector correction = FVector(90.f, 0.f, 0.f);
		p0 = RotatePointAroundPivot(p0, endPoint, (endRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());
		p1 = RotatePointAroundPivot(p1, endPoint, (endRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());

		orientation = endPoint - startPoint;
		orientation.Normalize();
		UE_LOG(LogClass, Log, TEXT("orientation %s"), *orientation.ToString());
		//if(orientation == FVector::ZeroVector)

			if (FMath::Abs(orientation.Z) > FMath::Abs(orientation.Y) && FMath::Abs(orientation.Z) > FMath::Abs(orientation.X)) {
				float angle = 45.f;
				if (endPoint.Z > startPoint.Z) {
					angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(startRotation, FVector::UpVector)));
					correction = FVector(180.f, -180.f, 0);
					correction.X -= ((45.f - angle) * 2);
					UE_LOG(LogClass, Log, TEXT("correction >"));

				}
				else if (endPoint.Z < startPoint.Z) {
					float angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(startRotation, -FVector::UpVector)));
					correction = FVector(0.f, 180.f, 0.f);
					correction.X += ((45.f - angle) * 2);
					UE_LOG(LogClass, Log, TEXT("correction <"));
				}

			}


		p2 = RotatePointAroundPivot(p2, startPoint, (startRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());
		p3 = RotatePointAroundPivot(p3, startPoint, (startRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());

		BuildQuad(p0, p1, p3, p2, VertexOffset, TriangleOffset, normal, tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 2] = FVector2D((VMapPerQuad * QuadIndex), 1.0f);
			UV0s[UV0s.Num() - 1] = FVector2D((VMapPerQuad * (QuadIndex + 1)), 1.0f);
			UV0s[UV0s.Num() - 4] = FVector2D((VMapPerQuad * (QuadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 3] = FVector2D((VMapPerQuad * QuadIndex), 0.0f);
		}

		// Calculate face normal
		FVector NormalCurrent = FVector::CrossProduct(p1 - p3, p2 - p3).GetSafeNormal();

		if (smoothNormals)
		{
			
			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			normals[normals.Num() - 4] = (p0 - endPoint).GetSafeNormal();;
			normals[normals.Num() - 3] = (p1 - endPoint).GetSafeNormal();;
			normals[normals.Num() - 2] = (p3 - startPoint).GetSafeNormal();;
			normals[normals.Num() - 1] = (p2 - startPoint).GetSafeNormal();;


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
				VertexOffset,
				TriangleOffset,
				normal,
				tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));

			//NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();
			NormalCurrent = -startRotation.GetSafeNormal();
			normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;
			
			// Tangents (perpendicular to the surface)
			FVector SurfaceTangent = p1 - p0;
			SurfaceTangent = SurfaceTangent.GetSafeNormal();
			
			
			// End cap
			BuildTriangle(
				p1, p0, pInitEnd, 
				VertexOffset,
				TriangleOffset,
				normal,
				tangent);

			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));

			NormalCurrent = endRotation.GetSafeNormal();
			normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = NormalCurrent;
			
		}
	}
}