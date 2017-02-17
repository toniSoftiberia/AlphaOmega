// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralMesh.h"
#include "ProceduralUtils.h"
#include "ProceduralLandscape.h"


#if WITH_EDITOR  
/** Creates the base componentes for the mesh and cleans all sections*/
AProceduralMesh::AProceduralMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a sphere component to see where the object  is in editor and make it root component
	//DummyComp = CreateDefaultSubobject<USceneComponent>(TEXT("DummyComp"));
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;

	// Creates the procedural mesh component and attach it to the root component
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	//mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


	// Cleans all sections to let it ready for use
	mesh->ClearAllMeshSections();
	mesh->ClearAllCachedCookedPlatformData();
}


/** Used for editing meshes on realtime*/
void AProceduralMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// We need to re-construct the buffers since values can be changed in editor
	vertices.Empty();
	UV0s.Empty();
	normals.Empty();
	tangents.Empty();
	triangles.Empty();
	vertexOffset = 0;

	GenerateMesh();

	// We also clean the mesh	
	mesh->ClearAllMeshSections();
	mesh->ClearAllCachedCookedPlatformData();

	// only create mesh if there is something to create
	if (vertices.Num() > 2) {

		mesh->CreateMeshSection(0, vertices, triangles, normals, UV0s, TArray<FColor>(), tangents, false);

		mesh->SetMaterial(0, material);
	}
}
#endif


// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();

	GenerateMesh();

	// We also clean the mesh	
	mesh->ClearAllMeshSections();
	mesh->ClearAllCachedCookedPlatformData();

	mesh->CreateMeshSection(0, vertices, triangles, normals, UV0s, TArray<FColor>(), tangents, false);

	mesh->SetMaterial(0, material);
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


/** Generates a quad from input vertices*/
void AProceduralMesh::BuildQuadOriented(FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft) {

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
	// We calculate how to divide the triangle
	if (bottomRight.Z > FMath::Max(bottomLeft.Z, topRight.Z) || topLeft.Z > FMath::Max(bottomLeft.Z, topRight.Z)) {
		triangles.Add(index1);
		triangles.Add(index2);
		triangles.Add(index3);
		triangles.Add(index1);
		triangles.Add(index3);
		triangles.Add(index4);
	}
	else {
		triangles.Add(index1);
		triangles.Add(index2);
		triangles.Add(index4);
		triangles.Add(index2);
		triangles.Add(index3);
		triangles.Add(index4);
	}

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
	BuildQuad(p0, p1, p2, p3);

	// Back (-X) face: 5-4-7-6
	BuildQuad(p5, p4, p7, p6);

	// Left (-Y) face: 1-5-6-2
	BuildQuad(p1, p5, p6, p2);

	// Right (+Y) face: 4-0-3-7
	BuildQuad(p4, p0, p3, p7);

	// Top (+Z) face: 6-7-3-2
	BuildQuad(p6, p7, p3, p2);

	// Bottom (-Z) face: 1-0-4-5
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
		}
	}
}


/** Generates a sphere from input values*/
void AProceduralMesh::BuildUVSphere(FVector center, float radius, int32 circleSections, int32 heightSections, bool smoothNormals, bool useUniqueTexture) {

	// Calculates angle between quads an UV0s for latitude and altitude
	const float angleBetweenAltitude = 360.0f / (float)circleSections;
	const float vMapPerAltitude = 1.0f / (float)circleSections;
	const float angleBetweenLatitude = (180.0f / (float)heightSections);
	const float vMapPerLatitude = 1.0f / (float)heightSections;

	// Calculetes the start point of all vertex, later we will use it rateted
	FVector pInitStart = FVector(FMath::Cos(0) * radius, FMath::Sin(0) * radius, 0.f) + center;

	// Generates all the circle sections
	for (int32 i = 0; i < circleSections; ++i) {

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
			if (useUniqueTexture) {

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
			if (smoothNormals) {

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


/** Subdivide the triangles for get more detail on geodesic sphere*/
void AProceduralMesh::SubdivideTriangle(const FVector &v1, const FVector &v2, const FVector &v3, const unsigned int depth, FVector center, float radius, bool smoothNormals) {
	
	// If we are in the final loop, we have to draw the triangle
	if (depth == 0) {

		BuildTriangle(v1* radius + center, v2* radius + center, v3* radius + center);

		// Apply smooth from the center of the sphere to the point
		if (smoothNormals) {
			normals[normals.Num() - 3] = (v1 * radius - center).GetSafeNormal();
			normals[normals.Num() - 2] = (v2 * radius - center).GetSafeNormal();
			normals[normals.Num() - 1] = (v3 * radius - center).GetSafeNormal();
		}
	}
	else {

		// Calculate new points for division
		const FVector v12 = (v1 + v2).GetSafeNormal();
		const FVector v23 = (v2 + v3).GetSafeNormal();
		const FVector v31 = (v3 + v1).GetSafeNormal();

		// Create the divisions
		SubdivideTriangle(v1, v12, v31, depth - 1, center, radius, smoothNormals);
		SubdivideTriangle(v2, v23, v12, depth - 1, center, radius, smoothNormals);
		SubdivideTriangle(v3, v31, v23, depth - 1, center, radius, smoothNormals);
		SubdivideTriangle(v12, v23, v31, depth - 1, center, radius, smoothNormals);
	}

}


/** Generates a sphere from input values*/
void AProceduralMesh::BuildGeodesicSphere(FVector center, float radius, const unsigned int depth, bool smoothNormals){
	
	// We build the icosa hardcoding the first loop of index and then dividing the triangles

	// Constant values needed to easy calculate vectors
	const float X = 0.525731112119133606;
	const float Z = 0.850650808352039932;

	// build first icosa
	const FVector vdata[12] = {
		{ -X, 0.0, Z },{ X, 0.0, Z },{ -X, 0.0, -Z },{ X, 0.0, -Z },
		{ 0.0, Z, X },{ 0.0, Z, -X },{ 0.0, -Z, X },{ 0.0, -Z, -X },
		{ Z, X, 0.0 },{ -Z, X, 0.0 },{ Z, -X, 0.0 },{ -Z, -X, 0.0 }
	};
	int tindices[20][3] = {
		{ 0, 4, 1 },{ 0, 9, 4 },{ 9, 5, 4 },{ 4, 5, 8 },{ 4, 8, 1 },
		{ 8, 10, 1 },{ 8, 3, 10 },{ 5, 3, 8 },{ 5, 2, 3 },{ 2, 7, 3 },
		{ 7, 10, 3 },{ 7, 6, 10 },{ 7, 11, 6 },{ 11, 0, 6 },{ 0, 1, 6 },
		{ 6, 1, 10 },{ 9, 0, 11 },{ 9, 11, 2 },{ 9, 2, 5 },{ 7, 2, 11 }
	};

	// Divide triangles
	for (int i = 0; i < 20; i++)
		SubdivideTriangle(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], depth, center, radius, smoothNormals);
}

/** Generates a tube from input values*/
void AProceduralMesh::BuildTubeFromOrientation(FVector startPoint, FVector endPoint, float radius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addStartCap, bool addEndCap) {

	FVector startRotation = (startPoint - endPoint).GetSafeNormal();
	FVector endRotation = (endPoint - startPoint).GetSafeNormal();

	startRotation = startRotation.Rotation().Add(90.f, 0.f, 0.f).Euler();
	endRotation = startRotation.Rotation().Add(90.f, 0.f, 0.f).Euler();

	BuildTube(startPoint, endPoint, startRotation, startRotation, radius, radius, circleSections, smoothNormals, useUniqueTexture, addStartCap, addEndCap);
}


/** Generates a tube from input values*/
void AProceduralMesh::BuildTubeFromRotations(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addStartCap, bool addEndCap) {
	
	BuildTube(startPoint, endPoint, startRotation, endRotation, startRadius, endRadius, circleSections, smoothNormals, useUniqueTexture, addStartCap, addEndCap);

}

/** Generates a tube from input values*/
void AProceduralMesh::BuildTube(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addStartCap, bool addEndCap) {


	// Calculates angle between quads an UV0s for for each face
	const float angleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float vMapPerQuad = 1.0f / (float)circleSections;

	// Store the init point and rotate it to use it later for caps
	FVector pInitStart = FVector(FMath::Cos(0) * startRadius, FMath::Sin(0) * startRadius, 0.f) + startPoint;
	FVector pInitEnd = FVector(FMath::Cos(0) * endRadius, FMath::Sin(0) * endRadius, 0.f) + endPoint;
	pInitStart = UProceduralUtils::RotatePointAroundPivot(pInitStart, startPoint, startRotation);
	pInitEnd = UProceduralUtils::RotatePointAroundPivot(pInitEnd, endPoint, endRotation);

	// Create all the sides of the tube
	for (int32 quadIndex = 0; quadIndex < circleSections; quadIndex++) {

		// Calculates desired angle and nextangle of every loop
		float angle = (float)quadIndex * angleBetweenQuads;
		float nextAngle = (float)(quadIndex + 1) * angleBetweenQuads;

		// Set up the vertices
		FVector p0 = (FVector(FMath::Cos(angle) * endRadius, FMath::Sin(angle) * endRadius, 0.f)) + endPoint;
		FVector p1 = (FVector(FMath::Cos(nextAngle) * endRadius, FMath::Sin(nextAngle) * endRadius, 0.f)) + endPoint;
		FVector p2 = (FVector(FMath::Cos(nextAngle) * startRadius, FMath::Sin(nextAngle) * startRadius, 0.f)) + startPoint;
		FVector p3 = (FVector(FMath::Cos(angle) * startRadius, FMath::Sin(angle) * startRadius, 0.f)) + startPoint;

		// We have the direction of the tube, for the faces we need to rotate it 90 degrees

		// Rotate all the vertices, we only apply correction to start point vertices
		p0 = UProceduralUtils::RotatePointAroundPivot(p0, endPoint, endRotation);
		p1 = UProceduralUtils::RotatePointAroundPivot(p1, endPoint, endRotation);
		p2 = UProceduralUtils::RotatePointAroundPivot(p2, startPoint, startRotation);
		p3 = UProceduralUtils::RotatePointAroundPivot(p3, startPoint, startRotation);

		// Generate the quad
		BuildQuad(p0, p1, p2, p3);

		// If we use a unique texture, we need to recalculate the UVs using vMapPerQuad
		if (useUniqueTexture) {

			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D((vMapPerQuad * (quadIndex + 1)), 1.0f);
			UV0s[UV0s.Num() - 2] = FVector2D((vMapPerQuad * quadIndex), 1.0f);
			UV0s[UV0s.Num() - 3] = FVector2D((vMapPerQuad * quadIndex), 0.0f);
			UV0s[UV0s.Num() - 4] = FVector2D((vMapPerQuad * (quadIndex + 1)), 0.0f);
		}

		// If we smooth normals, we need to recalculate the normals
		if (smoothNormals) {

			// As the vertices are rotated, will take into account the rotation
			// For the start normal smooth we use the each vertex minus the startPoint nomalized
			// For the end normal smooth we use the each vertex minus the endPoint nomalized
			normals[normals.Num() - 4] = (p0 - endPoint).GetSafeNormal();
			normals[normals.Num() - 3] = (p1 - endPoint).GetSafeNormal();
			normals[normals.Num() - 2] = (p2 - startPoint).GetSafeNormal();
			normals[normals.Num() - 1] = (p3 - startPoint).GetSafeNormal();
		}



		// Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
		// A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
		if (quadIndex != 0 && addStartCap) {

			// Generate the triangles
			BuildTriangle(pInitStart, p3, p2);

			// Recalculate the UVS to use an unique texture
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-nextAngle) / 2.0f), 0.5f - (FMath::Sin(-nextAngle) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-angle) / 2.0f), 0.5f - (FMath::Sin(-angle) / 2.0f));
		}

		if (quadIndex != 0 && addEndCap) {

			// Generate the triangles
			BuildTriangle(p0, pInitEnd, p1);

			// Recalculate the UVS to use an unique texture
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(nextAngle) / 2.0f), 0.5f - (FMath::Sin(nextAngle) / 2.0f));
			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(angle) / 2.0f), 0.5f - (FMath::Sin(angle) / 2.0f));
		}
	}
}


/** Generates a landscape from input values*/
void AProceduralMesh::BuildLandscape(float sizeX, float sizeY, int32 widthSections, int32 lenghtSections, TArray<FFloatArray> heightValues, bool smoothNormals, bool useUniqueTexture) {
	
	// Skip 0 division
	if (widthSections > 0 && lenghtSections > 0) {

		// Define the size of the steps in each quad
		float widthStep = sizeX / widthSections;
		float lengthStep = sizeY / lenghtSections;
		float UVwidthStep = (float)1 / widthSections;
		float UVlengthStep = (float)1 / lenghtSections;

		// Doble loop to generate the quads
				// Divide the mesh in sections using widthStep & lengthStep
		for (int i = 0; i < widthSections; ++i) {
			for (int j = 0; j < lenghtSections; ++j) {

				// Generates the vertives with the precalculated heightValues
				FVector p0 = FVector(widthStep * i - (sizeX / 2), lengthStep * j - (sizeY / 2), heightValues[i][j]);
				FVector p1 = FVector(widthStep * i - (sizeX / 2), lengthStep * (j + 1) - (sizeY / 2), heightValues[i][j + 1]);
				FVector p2 = FVector(widthStep * (i + 1) - (sizeX / 2), lengthStep * (j + 1) - (sizeY / 2), heightValues[i + 1][j + 1]);
				FVector p3 = FVector(widthStep * (i + 1) - (sizeX / 2), lengthStep * j - (sizeY / 2), heightValues[i + 1][j]);

				BuildQuadOriented(p0, p1, p2, p3);

				// If we use a unique texture, we need to recalculate the UVs using UVwidthStep & UVlengthStep
				if (useUniqueTexture) {

					// UVs.  Note that Unreal UV origin (0,0) is top left
					// Becarefull X and Y are inverted respect of the mesh representation
					UV0s[UV0s.Num() - 1] = FVector2D(UVlengthStep * (j), 1 - (UVwidthStep * (i + 1)));
					UV0s[UV0s.Num() - 2] = FVector2D(UVlengthStep * ((j + 1)), 1 - (UVwidthStep * (i + 1)));
					UV0s[UV0s.Num() - 3] = FVector2D(UVlengthStep * ((j + 1)), 1 - (UVwidthStep *((i))));
					UV0s[UV0s.Num() - 4] = FVector2D(UVlengthStep * (j), 1 - (UVwidthStep * ((i))));
				}

				// If we smooth normals, we need to recalculate the normals
				if (smoothNormals) {

					// For smooth we use an average of all the closer vertex
					// We have a method for get the precalculated smooth
					normals[normals.Num() - 4] = ((AProceduralLandscape*)this)->GetSmoothFromIndex(i, j);
					normals[normals.Num() - 3] = ((AProceduralLandscape*)this)->GetSmoothFromIndex(i, j + 1);
					normals[normals.Num() - 2] = ((AProceduralLandscape*)this)->GetSmoothFromIndex(i + 1, j + 1);
					normals[normals.Num() - 1] = ((AProceduralLandscape*)this)->GetSmoothFromIndex(i + 1, j);
				}
			}

		}
	}
}
