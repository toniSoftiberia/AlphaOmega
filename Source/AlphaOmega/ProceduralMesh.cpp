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
	//mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

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

	//mesh->UpdateMeshSection(1, vertices, normals, UV0, vertexColors, tangents);

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

	//UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh", meta = (AutoCreateRefTerm = "Normals,UV0,VertexColors,Tangents"))
	//	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
	// const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision);
}

void AProceduralMesh::BuildTriangle(
	FVector vertexA, FVector vertexB, FVector vertexC,
	int32& VertexOffset,
	int32& TriangleOffset,
	FVector& normal,
	FProcMeshTangent& tangent) {

	int32 Index1 = VertexOffset++;
	int32 Index2 = VertexOffset++;
	int32 Index3 = VertexOffset++;
	vertices.Add(vertexA);
	vertices.Add(vertexB);
	vertices.Add(vertexC);
	UV0s.Add(FVector2D(1.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 0.0f));
	UV0s.Add(FVector2D(0.0f, 1.0f));
	triangles.Add(Index1);
	triangles.Add(Index2);
	triangles.Add(Index3);
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
	int32& VertexOffset,
	int32& TriangleOffset,
	FVector& Normal,
	FProcMeshTangent& Tangent) {

	int32 Index1 = VertexOffset++;
	int32 Index2 = VertexOffset++;
	int32 Index3 = VertexOffset++;
	int32 Index4 = VertexOffset++;
	vertices.Add(bottomLeft);
	vertices.Add(bottomRight);
	vertices.Add(topRight);
	vertices.Add(topLeft);
	UV0s.Add(FVector2D(0.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 1.0f));
	UV0s.Add(FVector2D(1.0f, 0.0f));
	UV0s.Add(FVector2D(0.0f, 0.0f));
	triangles.Add(Index1);
	triangles.Add(Index2);
	triangles.Add(Index3);
	triangles.Add(Index1);
	triangles.Add(Index3);
	triangles.Add(Index4);
	// On a cube side, all the vertex normals face the same way
	normals.Add(Normal);
	normals.Add(Normal);
	normals.Add(Normal);
	normals.Add(Normal);
	tangents.Add(Tangent);
	tangents.Add(Tangent);
	tangents.Add(Tangent);
	tangents.Add(Tangent);
}

void AProceduralMesh::BuildCube(FVector cubeSize, int32& VertexOffset, int32& TriangleOffset, FProcMeshTangent& tangent){

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

	// Front (+X) face: 0-1-2-3
	FVector normal = FVector::ForwardVector;
	BuildQuad(p0, p1, p2, p3, VertexOffset, TriangleOffset, normal, tangent);

	// Back (-X) face: 5-4-7-6
	normal = -FVector::ForwardVector;
	BuildQuad(p5, p4, p7, p6, VertexOffset, TriangleOffset, normal, tangent);

	// Left (-Y) face: 1-5-6-2
	normal = -FVector::RightVector;
	BuildQuad(p1, p5, p6, p2, VertexOffset, TriangleOffset, normal, tangent);

	// Right (+Y) face: 4-0-3-7
	normal = FVector::RightVector;
	BuildQuad(p4, p0, p3, p7, VertexOffset, TriangleOffset, normal, tangent);

	// Top (+Z) face: 6-7-3-2
	normal = FVector::UpVector;
	BuildQuad(p6, p7, p3, p2, VertexOffset, TriangleOffset, normal, tangent);

	// Bottom (-Z) face: 1-0-4-5
	normal = -FVector::UpVector;
	BuildQuad(p1, p0, p4, p5, VertexOffset, TriangleOffset, normal, tangent);
}

void AProceduralMesh::BuildPiramid(float height, float radius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addBottomCap,
	int32& VertexOffset, int32& TriangleOffset, FProcMeshTangent& Tangent) {

	
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

		BuildTriangle(p1, p0, Offset, VertexOffset, TriangleOffset, NormalCurrent, Tangent);

		if (useUniqueTexture)
		{
			// UVs.  Note that Unreal UV origin (0,0) is top left
			UV0s[UV0s.Num() - 1] = FVector2D(1.0f - (VMapPerQuad), 1.0f);
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
		if (QuadIndex != 0 && addBottomCap)
		{

			NormalCurrent = FVector::CrossProduct(vertices[vertices.Num() - 3] - vertices[vertices.Num() - 1], vertices[vertices.Num() - 2] - vertices[vertices.Num() - 1]).GetSafeNormal();

			// Cap
			BuildTriangle(
				pInit, p0, p1,
				VertexOffset,
				TriangleOffset,
				NormalCurrent,
				Tangent);

			UV0s[UV0s.Num() - 3] = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
			UV0s[UV0s.Num() - 2] = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));
			UV0s[UV0s.Num() - 1] = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));

			// Tangents (perpendicular to the surface)
			FVector SurfaceTangent = p0 - p1;
			SurfaceTangent = SurfaceTangent.GetSafeNormal();

		}
	}
}



void AProceduralMesh::GenerateSphere(FVector center, float radius, int32 circleSections, int32 heightSections, bool smoothNormals, bool useUniqueTexture,
	int32 &VertexOffset, int32 &TriangleOffset, FProcMeshTangent tangent) {

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
				UV0s[UV0s.Num() - 5] = FVector2D(1.0f - (VMapPerAltitude * i), 0.5f - (VMapPerLatitude * j));
				UV0s[UV0s.Num() - 6] = FVector2D(1.0f - (VMapPerAltitude * (i + 1)), 0.5f - (VMapPerLatitude * j));
				UV0s[UV0s.Num() - 7] = FVector2D(1.0f - (VMapPerAltitude * (i + 1)), 0.5f - (VMapPerLatitude * (j + 1)));
				UV0s[UV0s.Num() - 8] = FVector2D(1.0f - (VMapPerAltitude * i), 0.5f - (VMapPerLatitude * (j + 1)));
			}
		}
	}
}


FVector AProceduralMesh::RotatePointAroundPivot(FVector InPoint, FVector InPivot, FVector InAngles)
{
	FVector direction = InPoint - InPivot; // get point direction relative to pivot
	direction = FQuat::MakeFromEuler(InAngles) * direction; // rotate it
	return direction + InPivot; // calculate rotated point
}
