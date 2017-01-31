// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralMesh.h"


#if WITH_EDITOR  
// Sets default values
AProceduralMesh::AProceduralMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;


	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));

	mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	mesh->ClearAllMeshSections();
	mesh->ClearAllCachedCookedPlatformData();

}

/*
void AProceduralMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// We need to re-construct the buffers since values can be changed in editor

	//GenerateMesh();

	//mesh->CreateMeshSection(1, vertices, triangles, normals, UV0s, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//mesh->UpdateMeshSection(1, vertices, normals, UV0, vertexColors, tangents);

	//mesh->SetMaterial(1, material);
}*/
#endif // WITH_EDITOR

// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();

	GenerateMesh();

	mesh->CreateMeshSection(1, vertices, triangles, normals, UV0s, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	mesh->SetMaterial(1, material);
}

// Called every frame
void AProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralMesh::GenerateMesh() {
	/**
	*	Create/replace a section for this procedural mesh component.
	*	@param	SectionIndex		Index of the section to create or replace.
	*	@param	Vertices			Vertex buffer of all vertex positions to use for this mesh section.
	*	@param	Triangles			Index buffer indicating which vertices make up each triangle. Length must be a multiple of 3.
	*	@param	Normals				Optional array of normal vectors for each vertex. If supplied, must be same length as Vertices array.
	*	@param	UV0					Optional array of texture co-ordinates for each vertex. If supplied, must be same length as Vertices array.
	*	@param	VertexColors		Optional array of colors for each vertex. If supplied, must be same length as Vertices array.
	*	@param	Tangents			Optional array of tangent vector for each vertex. If supplied, must be same length as Vertices array.
	*	@param	bCreateCollision	Indicates whether collision should be created for this section. This adds significant cost.
	*/

	// This is a pure virtual method allways defined in child classes
}

void AProceduralMesh::BuildTriangle(
	FVector vertexA, FVector vertexB, FVector vertexC,
	int32& vertexOffset, int32& triangleOffset, FVector& normal, FProcMeshTangent& tangent) {

	int32 index1 = vertexOffset++;
	int32 index2 = vertexOffset++;
	int32 index3 = vertexOffset++;
	vertices.Add(vertexA);
	vertices.Add(vertexB);
	vertices.Add(vertexC);
	UV0s.Add(FVector2D(1.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 0.0f));
	UV0s.Add(FVector2D(0.0f, 1.0f));
	triangles.Add(index1);
	triangles.Add(index2);
	triangles.Add(index3);
	// On a triangle side, all the vertex normals face the same way
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);
	tangents.Add(tangent);
	tangents.Add(tangent);
	tangents.Add(tangent);

}

void AProceduralMesh::BuildQuad(
	FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft,
	int32& vertexOffset, int32& triangleOffset, FVector& Normal, FProcMeshTangent& tangent) {

	int32 index1 = vertexOffset++;
	int32 index2 = vertexOffset++;
	int32 index3 = vertexOffset++;
	int32 index4 = vertexOffset++;
	vertices.Add(bottomLeft);
	vertices.Add(bottomRight);
	vertices.Add(topRight);
	vertices.Add(topLeft);
	UV0s.Add(FVector2D(0.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 0.0f));
	UV0s.Add(FVector2D(0.0f, 0.0f));
	triangles.Add(index1);
	triangles.Add(index2);
	triangles.Add(index3);
	triangles.Add(index1);
	triangles.Add(index3);
	triangles.Add(index4);
	// On a cube side, all the vertex normals face the same way
	normals.Add(Normal);
	normals.Add(Normal);
	normals.Add(Normal);
	normals.Add(Normal);
	tangents.Add(tangent);
	tangents.Add(tangent);
	tangents.Add(tangent);
	tangents.Add(tangent);
}

void AProceduralMesh::BuildCube(FVector cubeSize, int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent){

	// Calculate a half offset so we get correct center of object
	float offsetX = cubeSize.X / 2.0f;
	float offsetY = cubeSize.Y / 2.0f;
	float offsetZ = cubeSize.Z / 2.0f;

	// Define the 8 corners of the cube
	FVector p0 = FVector(offsetX, offsetY, -offsetZ);
	FVector p1 = FVector(offsetX, -offsetY, -offsetZ);
	FVector p2 = FVector(offsetX, -offsetY, offsetZ);
	FVector p3 = FVector(offsetX, offsetY, offsetZ);
	FVector p4 = FVector(-offsetX, offsetY, -offsetZ);
	FVector p5 = FVector(-offsetX, -offsetY, -offsetZ);
	FVector p6 = FVector(-offsetX, -offsetY, offsetZ);
	FVector p7 = FVector(-offsetX, offsetY, offsetZ);

	// Front (+X) face: 0-1-2-3
	FVector normal = FVector::ForwardVector;
	BuildQuad(p0, p1, p2, p3, vertexOffset, triangleOffset, normal, tangent);

	// Back (-X) face: 5-4-7-6
	normal = -FVector::ForwardVector;
	BuildQuad(p5, p4, p7, p6, vertexOffset, triangleOffset, normal, tangent);

	// Left (-Y) face: 1-5-6-2
	normal = -FVector::RightVector;
	BuildQuad(p1, p5, p6, p2, vertexOffset, triangleOffset, normal, tangent);

	// Right (+Y) face: 4-0-3-7
	normal = FVector::RightVector;
	BuildQuad(p4, p0, p3, p7, vertexOffset, triangleOffset, normal, tangent);

	// Top (+Z) face: 6-7-3-2
	normal = FVector::UpVector;
	BuildQuad(p6, p7, p3, p2, vertexOffset, triangleOffset, normal, tangent);

	// Bottom (-Z) face: 1-0-4-5
	normal = -FVector::UpVector;
	BuildQuad(p1, p0, p4, p5, vertexOffset, triangleOffset, normal, tangent);
}

void AProceduralMesh::BuildPiramid(float height, float radius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addBottomCap,
	int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent) {

	
	// Make a cylinder section
	const float angleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float vMapPerQuad = 1.0f / (float)circleSections;
	FVector offset = FVector(0, 0, height);

	FVector pInit = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f);

	for (int32 quadIndex = 0; quadIndex < circleSections; quadIndex++)
	{
		float angle = (float)quadIndex * angleBetweenQuads;
		float nextAngle = (float)(quadIndex + 1) * angleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(angle) * radius, FMath::Sin(angle) * radius, 0.f));
		FVector p1 = (FVector(FMath::Cos(nextAngle) * radius, FMath::Sin(nextAngle) * radius, 0.f));

		// Calculate face normal
		FVector currentNormal = FVector::CrossProduct(p0 - offset, p1 - offset).GetSafeNormal();

		BuildTriangle(p1, p0, offset, vertexOffset, triangleOffset, currentNormal, tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (vMapPerQuad), 1.0f);
			UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (vMapPerQuad * (quadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (vMapPerQuad * quadIndex), 0.0f);
		}


		if (smoothNormals)
		{

			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			normals[normals.Num() - 3] = (p1).GetSafeNormal();
			normals[normals.Num() - 2] = (p0).GetSafeNormal();
			normals[normals.Num() - 1] = FVector::UpVector;


		}

		// -------------------------------------------------------
		// Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
		// A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
		if (quadIndex != 0 && addBottomCap)
		{

			currentNormal = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();

			// Cap
			BuildTriangle(
				pInit, p0, p1,
				vertexOffset,
				triangleOffset,
				currentNormal,
				tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));

			// Tangents (perpendicular to the surface)
			FVector surfaceTangent = p0 - p1;
			surfaceTangent = surfaceTangent.GetSafeNormal();

		}
	}
}



void AProceduralMesh::BuildSphere(FVector center, float radius, int32 circleSections, int32 heightSections, bool smoothNormals, bool useUniqueTexture,
	int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent) {

	const float angleBetweenAltitude = 360.0f / (float)circleSections;
	const float vMapPerAltitude = 1.0f / (float)circleSections;
	const float angleBetweenLatitude = (180.0f / (float)heightSections);
	const float vMapPerLatitude = 1.0f / (float)heightSections;

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
			BuildQuad(p3, p2, p1, p0, vertexOffset, triangleOffset, currentNormal, tangent);

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
			BuildQuad(p3, p2, p1, p0, vertexOffset, triangleOffset, currentNormal, tangent);

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
				UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (vMapPerAltitude * (i + 1)), 0.5f + (vMapPerLatitude * j));
				UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (vMapPerAltitude * i), 0.5f + (vMapPerLatitude * j));
				UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (vMapPerAltitude * i), 0.5f + (vMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 4] = FVector2D(1.0f - (vMapPerAltitude * (i + 1)), 0.5f + (vMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 5] = FVector2D(1.0f - (vMapPerAltitude * i), 0.5f - (vMapPerLatitude * j));
				UV0s[UV0s.Num() - 6] = FVector2D(1.0f - (vMapPerAltitude * (i + 1)), 0.5f - (vMapPerLatitude * j));
				UV0s[UV0s.Num() - 7] = FVector2D(1.0f - (vMapPerAltitude * (i + 1)), 0.5f - (vMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 8] = FVector2D(1.0f - (vMapPerAltitude * i), 0.5f - (vMapPerLatitude * (j + 1)));
			}
		}
	}
}


void AProceduralMesh::BuildTube(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addCaps,
	int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent) {
	// Get prisma orientation and flip it to get its perpendicular
	FVector orientation = FVector::ZeroVector;

	bool comingFromSimpleTube = false;

	if (endRotation == FVector::ZeroVector || startRotation == FVector::ZeroVector) {
		orientation = endPoint - startPoint;
		startRotation = orientation;
		endRotation = orientation;
		comingFromSimpleTube = true;
	}

	// Make a cylinder section
	const float angleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float vMapPerQuad = 1.0f / (float)circleSections;

	FVector pInitStart = FVector(FMath::Cos(0) * startRadius, FMath::Sin(0) * startRadius, 0.f) + startPoint;
	FVector pInitEnd = FVector(FMath::Cos(0) * endRadius, FMath::Sin(0) * endRadius, 0.f) + endPoint;
	pInitStart = RotatePointAroundPivot(pInitStart, startPoint, (startRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());
	pInitEnd = RotatePointAroundPivot(pInitEnd, endPoint, (endRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());

	for (int32 quadIndex = 0; quadIndex < circleSections; quadIndex++)
	{
		float angle = (float)quadIndex * angleBetweenQuads;
		float nextAngle = (float)(quadIndex + 1) * angleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(nextAngle) * endRadius, FMath::Sin(nextAngle) * endRadius, 0.f)) + endPoint;
		FVector p1 = (FVector(FMath::Cos(angle) * endRadius, FMath::Sin(angle) * endRadius, 0.f)) + endPoint;
		FVector p2 = (FVector(FMath::Cos(nextAngle) * startRadius, FMath::Sin(nextAngle) * startRadius, 0.f)) + startPoint;
		FVector p3 = (FVector(FMath::Cos(angle) * startRadius, FMath::Sin(angle) * startRadius, 0.f)) + startPoint;

		FVector correction = FVector(90.f, 0.f, 0.f);
		p0 = RotatePointAroundPivot(p0, endPoint, (endRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());
		p1 = RotatePointAroundPivot(p1, endPoint, (endRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());

		orientation = endPoint - startPoint;
		orientation.Normalize();

		if (!comingFromSimpleTube) {
			if (FMath::Abs(orientation.Z) > FMath::Abs(orientation.Y) && FMath::Abs(orientation.Z) > FMath::Abs(orientation.X)) {
				float angle = 45.f;
				if (endPoint.Z > startPoint.Z) {
					angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(startRotation, FVector::UpVector)));
					correction = FVector(180.f, -180.f, 0);
					correction.X -= ((45.f - angle) * 2);

				}
				else if (endPoint.Z < startPoint.Z) {
					angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(startRotation, -FVector::UpVector)));
					correction = FVector(0.f, 180.f, 0.f);
					correction.X += ((45.f - angle) * 2);
				}

			}
		}

		p2 = RotatePointAroundPivot(p2, startPoint, (startRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());
		p3 = RotatePointAroundPivot(p3, startPoint, (startRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());


		FVector currentNormal = FVector::CrossProduct(p1 - p3, p2 - p3).GetSafeNormal();

		BuildQuad(p0, p1, p3, p2, vertexOffset, triangleOffset, currentNormal, tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 2] = FVector2D((vMapPerQuad * quadIndex), 1.0f);
			UV0s[UV0s.Num() - 1] = FVector2D((vMapPerQuad * (quadIndex + 1)), 1.0f);
			UV0s[UV0s.Num() - 4] = FVector2D((vMapPerQuad * (quadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 3] = FVector2D((vMapPerQuad * quadIndex), 0.0f);
		}

		// Calculate face normal

		if (smoothNormals)
		{

			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			normals[normals.Num() - 4] = (p0 - endPoint).GetSafeNormal();
			normals[normals.Num() - 3] = (p1 - endPoint).GetSafeNormal();
			normals[normals.Num() - 2] = (p3 - startPoint).GetSafeNormal();
			normals[normals.Num() - 1] = (p2 - startPoint).GetSafeNormal();


		}
		else
		{
			// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
			normals[normals.Num() - 4] = normals[normals.Num() - 3] = normals[normals.Num() - 2] = normals[normals.Num() - 1] = currentNormal;
		}


		// -------------------------------------------------------
		// Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
		// A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
		if (quadIndex != 0 && addCaps)
		{

			// Start cap
			currentNormal = -startRotation.GetSafeNormal();

			BuildTriangle(
				pInitStart, p2, p3,
				vertexOffset,
				triangleOffset,
				currentNormal,
				tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));

			// Tangents (perpendicular to the surface)
			FVector surfaceTangent = p1 - p0;
			surfaceTangent = surfaceTangent.GetSafeNormal();



			// End cap
			currentNormal = endRotation.GetSafeNormal();

			BuildTriangle(
				p1, p0, pInitEnd,
				vertexOffset,
				triangleOffset,
				currentNormal,
				tangent);

			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));

		}
	}
}

FVector AProceduralMesh::RotatePointAroundPivot(FVector InPoint, FVector InPivot, FVector InAngles)
{
	FVector direction = InPoint - InPivot; // get point direction relative to pivot
	direction = FQuat::MakeFromEuler(InAngles) * direction; // rotate it
	return direction + InPivot; // calculate rotated point
}
