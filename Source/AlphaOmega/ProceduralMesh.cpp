// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralMesh.h"
#include "ProceduralUtils.h"


#if WITH_EDITOR  
/** Creates the base componentes for the mesh and cleans all sections*/
AProceduralMesh::AProceduralMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a sphere component to see where the object  is in editor and make it root component
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;

	// Creates the procedural mesh component and attach it to the root component
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Cleans all sections to let it ready for use
	mesh->ClearAllMeshSections();
	mesh->ClearAllCachedCookedPlatformData();

}


/** Used for editing meshes on realtime
void AProceduralMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// We need to re-construct the buffers since values can be changed in editor

	//GenerateMesh();

	//mesh->CreateMeshSection(1, vertices, triangles, normals, UV0s, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//mesh->UpdateMeshSection(1, vertices, normals, UV0, vertexColors, tangents);

	//mesh->SetMaterial(1, material);
}*/
#endif


// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();

	GenerateMesh();

	mesh->CreateMeshSection(1, vertices, triangles, normals, UV0s, TArray<FColor>(), tangents, false);

	mesh->SetMaterial(1, material);
}


// Called every frame
void AProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/** Generates the mesh that we have in our local variables*/
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


/** Generates a triangle from input vertices*/
void AProceduralMesh::BuildTriangle(FVector vertexA, FVector vertexB, FVector vertexC) {

	vertices.Add(vertexA);
	vertices.Add(vertexB);
	vertices.Add(vertexC);
	
	UV0s.Add(FVector2D(1.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 0.0f));
	UV0s.Add(FVector2D(0.0f, 1.0f));
	
	triangles.Add(vertexOffset++);
	triangles.Add(vertexOffset++);
	triangles.Add(vertexOffset++);

	// By default all normals look at the same way
	FVector normal = FVector::CrossProduct(vertexC - vertexA, vertexB - vertexA).GetSafeNormal();
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	// Tangents (perpendicular to the surface)
	FVector SurfaceTangent = vertexA - vertexA;
	SurfaceTangent = SurfaceTangent.GetSafeNormal();
	FProcMeshTangent tangent = FProcMeshTangent(SurfaceTangent, true);

	tangents.Add(tangent);
	tangents.Add(tangent);
	tangents.Add(tangent);

}


/** Generates a quad from input vertices*/
void AProceduralMesh::BuildQuad(FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft) {

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

	// Here we have to indicate 2 triangles
	triangles.Add(index1);
	triangles.Add(index2);
	triangles.Add(index3);
	triangles.Add(index1);
	triangles.Add(index3);
	triangles.Add(index4);

	// By default all normals look at the same way
	FVector	normal = FVector::CrossProduct(topRight - topLeft, bottomLeft - topLeft).GetSafeNormal();
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);
	normals.Add(normal);

	// Tangents (perpendicular to the surface)
	FVector SurfaceTangent = bottomLeft - bottomRight;
	SurfaceTangent = SurfaceTangent.GetSafeNormal();
	FProcMeshTangent tangent = FProcMeshTangent(SurfaceTangent, true);
	tangents.Add(tangent);
	tangents.Add(tangent);
	tangents.Add(tangent);
	tangents.Add(tangent);
}


/** Generates a cube of cubesize size*/
void AProceduralMesh::BuildCube(FVector cubeSize){

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
	BuildQuad(p0, p1, p2, p3);

	// Back (-X) face: 5-4-7-6
	normal = -FVector::ForwardVector;
	BuildQuad(p5, p4, p7, p6);

	// Left (-Y) face: 1-5-6-2
	normal = -FVector::RightVector;
	BuildQuad(p1, p5, p6, p2);

	// Right (+Y) face: 4-0-3-7
	normal = FVector::RightVector;
	BuildQuad(p4, p0, p3, p7);

	// Top (+Z) face: 6-7-3-2
	normal = FVector::UpVector;
	BuildQuad(p6, p7, p3, p2);

	// Bottom (-Z) face: 1-0-4-5
	normal = -FVector::UpVector;
	BuildQuad(p1, p0, p4, p5);
}


/** Generates a piramid from input values*/
void AProceduralMesh::BuildPiramid(float height, float radius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addBottomCap) {

	
	// Calculates angle between quads an UV0s
	const float angleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float vMapPerQuad = 1.0f / (float)circleSections;

	// Calculates the top vertex of the piramid
	FVector offset = FVector(0, 0, height);

	// Store the init point to use it later for cap
	FVector pInit = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f);

	// Create all the sides of the piramid
	for (int32 triIndex = 0; triIndex < circleSections; triIndex++)	{

		// Calculates desired angle and nextangle of every loop
		float angle = (float)triIndex * angleBetweenQuads;
		float nextAngle = (float)(triIndex + 1) * angleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(angle) * radius, FMath::Sin(angle) * radius, 0.f));
		FVector p1 = (FVector(FMath::Cos(nextAngle) * radius, FMath::Sin(nextAngle) * radius, 0.f));

		// Generate the triangle
		BuildTriangle(p1, p0, offset);

		// If we use a unique texture, we need to recalculate the UVs using vMapPerQuad
		if (useUniqueTexture){

			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (vMapPerQuad), 1.0f);
			UV0s[UV0s.Num() - 3] = FVector2D(1.0f - (vMapPerQuad * (triIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 2] = FVector2D(1.0f - (vMapPerQuad * triIndex), 0.0f);
		}

		// If we smooth normals, we need to recalculate normals
		if (smoothNormals){

			// For the bottom vertices we use itself normalized
			// For the top vertex we use UpVector
			normals[normals.Num() - 3] = (p1).GetSafeNormal();
			normals[normals.Num() - 2] = (p0).GetSafeNormal();
			normals[normals.Num() - 1] = FVector::UpVector;
		}

		// We only need one cap in this figure the first triangle is not drawed
		if (triIndex != 0 && addBottomCap)
		{
			// Generate the triangle
			BuildTriangle( pInit, p0, p1);

			// Change the default UV0s to use an unique texture
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));

			/*
			// Tangents (perpendicular to the surface)
			FVector surfaceTangent = p0 - p1;
			surfaceTangent = surfaceTangent.GetSafeNormal();
			*/
		}
	}
}


/** Generates a sphere from input values*/
void AProceduralMesh::BuildSphere(FVector center, float radius, int32 circleSections, int32 heightSections, bool smoothNormals, bool useUniqueTexture) {

	// Calculates angle between quads an UV0s for latitude and altitude
	const float angleBetweenAltitude = 360.0f / (float)circleSections;
	const float vMapPerAltitude = 1.0f / (float)circleSections;
	const float angleBetweenLatitude = (180.0f / (float)heightSections);
	const float vMapPerLatitude = 1.0f / (float)heightSections;

	// Calculetes the start point of all vertex, later we will use it rateted
	FVector pInitStart = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f) + center;

	// Generates all the circle sections
	for (int32 i = 0; i < circleSections; ++i){

		// Calculates desired angle and nextangle for each circle section
		float angleAltitude = (float)i * angleBetweenAltitude;
		float nextAngleAltitude = (float)(i + 1) * angleBetweenAltitude;

		// Generates all quads of a circle section
		for (int32 j = 0; j < heightSections / 2; ++j) {

			// Calculates desired angle and nextangle for each quad of section
			float angleLatitude = (float)j * angleBetweenLatitude;
			float nextAngleLatitude = (float)(j + 1) * angleBetweenLatitude;

			// Set up the vertices we generate the top and bottom hemisfere at the same time
			FVector p0 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, angleLatitude, angleAltitude));
			FVector p1 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, angleLatitude, nextAngleAltitude));
			FVector p2 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, nextAngleLatitude, nextAngleAltitude));
			FVector p3 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, nextAngleLatitude, angleAltitude));
			FVector p4 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, -angleLatitude, nextAngleAltitude));
			FVector p5 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, -angleLatitude, angleAltitude));
			FVector p6 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, -nextAngleLatitude, angleAltitude));
			FVector p7 = UProceduralUtils::RotatePointAroundPivot(pInitStart, center, FVector(.0f, -nextAngleLatitude, nextAngleAltitude));
			
			// Generate the quads
			BuildQuad(p3, p2, p1, p0);
			BuildQuad(p7, p6, p5, p4);

			// If we use a unique texture, we need to recalculate the UVs using vMapPerQuad
			if (useUniqueTexture){

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

			// If we smooth normals, we need to recalculate normals
			if (smoothNormals){

				// For the normal smooth we use the each vertex minus the center nomalized
				normals[normals.Num() - 8] = (p3 - center).GetSafeNormal();
				normals[normals.Num() - 7] = (p2 - center).GetSafeNormal();
				normals[normals.Num() - 6] = (p1 - center).GetSafeNormal();
				normals[normals.Num() - 5] = (p0 - center).GetSafeNormal();
				normals[normals.Num() - 4] = (p7 - center).GetSafeNormal();
				normals[normals.Num() - 3] = (p6 - center).GetSafeNormal();
				normals[normals.Num() - 2] = (p5 - center).GetSafeNormal();
				normals[normals.Num() - 1] = (p4 - center).GetSafeNormal();
			}
		}
	}
}

/** Generates a tube from input values*/
void AProceduralMesh::BuildTube(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addCaps) {

	// Get prisma orientation and flip it to get its perpendicular
	FVector orientation = endPoint - startPoint;

	// Hanldes if we are drawing a unique tube
	bool comingFromSimpleTube = false;

	// Checks if we are drawing a unique tube, if it is, we need to change the value of startRotation and endRotation
	if (endRotation == FVector::ZeroVector || startRotation == FVector::ZeroVector) {
		orientation = endPoint - startPoint;
		startRotation = orientation;
		endRotation = orientation;
		comingFromSimpleTube = true;
	}

	// Calculates angle between quads an UV0s for for each face
	const float angleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float vMapPerQuad = 1.0f / (float)circleSections;

	// Store the init point and rotate it to use it later for caps
	FVector pInitStart = FVector(FMath::Cos(0) * startRadius, FMath::Sin(0) * startRadius, 0.f) + startPoint;
	FVector pInitEnd = FVector(FMath::Cos(0) * endRadius, FMath::Sin(0) * endRadius, 0.f) + endPoint;
	pInitStart = UProceduralUtils::RotatePointAroundPivot(pInitStart, startPoint, (startRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());
	pInitEnd = UProceduralUtils::RotatePointAroundPivot(pInitEnd, endPoint, (endRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());

	// Create all the sides of the tube
	for (int32 quadIndex = 0; quadIndex < circleSections; quadIndex++){

		// Calculates desired angle and nextangle of every loop
		float angle = (float)quadIndex * angleBetweenQuads;
		float nextAngle = (float)(quadIndex + 1) * angleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(nextAngle) * endRadius, FMath::Sin(nextAngle) * endRadius, 0.f)) + endPoint;
		FVector p1 = (FVector(FMath::Cos(angle) * endRadius, FMath::Sin(angle) * endRadius, 0.f)) + endPoint;
		FVector p2 = (FVector(FMath::Cos(nextAngle) * startRadius, FMath::Sin(nextAngle) * startRadius, 0.f)) + startPoint;
		FVector p3 = (FVector(FMath::Cos(angle) * startRadius, FMath::Sin(angle) * startRadius, 0.f)) + startPoint;

		// We have the direction of the tube, for the faces we need to rotate it 90 degrees
		FVector correction = FVector(90.f, 0.f, 0.f);

		// Cause we allways rotate 90 degrees in X axis we need to correct it when orientation is bigger in Z axis
		// If we aren't drawing a simple tube and its orientation is bigger in Z axis
		if (!comingFromSimpleTube && FMath::Abs(orientation.Z) > FMath::Abs(orientation.Y) && FMath::Abs(orientation.Z) > FMath::Abs(orientation.X)) {
			float angleCorrection = 45.f;

			// For up to down tube orientations
			if (endPoint.Z > startPoint.Z) {
				angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(startRotation, FVector::UpVector)));
				correction = FVector(180.f, -180.f, 0);
				correction.X -= ((45.f - angle) * 2);
			}
			// For down to up tube orientations
			else if (endPoint.Z < startPoint.Z) {
				angle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(startRotation, -FVector::UpVector)));
				correction = FVector(0.f, 180.f, 0.f);
				correction.X += ((45.f - angle) * 2);
			}
		}

		// Rotate all the vertices, we only apply correction to start point vertices
		p0 = UProceduralUtils::RotatePointAroundPivot(p0, endPoint, (endRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());
		p1 = UProceduralUtils::RotatePointAroundPivot(p1, endPoint, (endRotation).Rotation().Add(90.f, 0.f, 0.f).Euler());
		p2 = UProceduralUtils::RotatePointAroundPivot(p2, startPoint, (startRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());
		p3 = UProceduralUtils::RotatePointAroundPivot(p3, startPoint, (startRotation).Rotation().Add(correction.X, correction.Y, correction.Z).Euler());
		
		// Generate the quad
		BuildQuad(p0, p1, p3, p2);

		// If we use a unique texture, we need to recalculate the UVs using vMapPerQuad
		if (useUniqueTexture){

			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 2] = FVector2D((vMapPerQuad * quadIndex), 1.0f);
			UV0s[UV0s.Num() - 1] = FVector2D((vMapPerQuad * (quadIndex + 1)), 1.0f);
			UV0s[UV0s.Num() - 4] = FVector2D((vMapPerQuad * (quadIndex + 1)), 0.0f);
			UV0s[UV0s.Num() - 3] = FVector2D((vMapPerQuad * quadIndex), 0.0f);
		}

		// If we smooth normals, we need to recalculate the normals
		if (smoothNormals){

			// As the vertices are rotated, will take into account the rotation
			// For the start normal smooth we use the each vertex minus the startPoint nomalized
			// For the end normal smooth we use the each vertex minus the endPoint nomalized
			normals[normals.Num() - 4] = (p0 - endPoint).GetSafeNormal();
			normals[normals.Num() - 3] = (p1 - endPoint).GetSafeNormal();
			normals[normals.Num() - 2] = (p3 - startPoint).GetSafeNormal();
			normals[normals.Num() - 1] = (p2 - startPoint).GetSafeNormal();
		}


		// Here we need a start cap and end cap
		// Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
		// A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
		if (quadIndex != 0 && addCaps){

			// Generate the triangles
			BuildTriangle(pInitStart, p2, p3);
			BuildTriangle(p1, p0, pInitEnd);

			// Recalculate the UVS to use an unique texture
			UV0s[UV0s.Num() - 6] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 5] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));
			UV0s[UV0s.Num() - 4] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));
			
			/*
			// Tangents (perpendicular to the surface)
			FVector surfaceTangent = p1 - p0;
			surfaceTangent = surfaceTangent.GetSafeNormal();*/
		}
	}
}
