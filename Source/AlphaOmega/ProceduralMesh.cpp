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
#endif // WITH_EDITOR

// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();

	//GenerateTriangle();

	//GenerateQuad();

	//GenerateCube(FVector(100, 100, 100));


	GenerateTube(50, 200, 10, 1);
}

// Called every frame
void AProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralMesh::GenerateTriangle() {

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

	TArray<FVector> vertices;
	vertices.Add(FVector(0, 0, 0)); // + RootComponent->RelativeLocation
	vertices.Add(FVector(0, 0, 100));
	vertices.Add(FVector(0, -100, 0));

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);

	TArray<FVector> normals;
	normals.Add(FVector(0, 0, 0));
	normals.Add(FVector(0, 0, 0));
	normals.Add(FVector(0, 0, 0));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(1, 1));
	UV0.Add(FVector2D(1, 0));
	UV0.Add(FVector2D(0, 1));

	TArray<FColor> vertexColors;
	vertexColors.Add(FColor(0, 0, 0, 0));
	vertexColors.Add(FColor(0, 0, 0, 0));
	vertexColors.Add(FColor(0, 0, 0, 0));


	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));


	mesh->CreateMeshSection(1, vertices, Triangles, normals, UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), false);

	//mesh->UpdateMeshSection(1, vertices, normals, UV0, vertexColors, tangents);

	mesh->SetMaterial(1, material);

	// With default options
	//mesh->CreateMeshSection(1, vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

}


void AProceduralMesh::GenerateQuad() {
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

	TArray<FVector> vertices;

	vertices.Add(FVector(0, 0, 0));
	vertices.Add(FVector(0, 100, 0));
	vertices.Add(FVector(0, 100, 100));
	vertices.Add(FVector(0, 0, 100));

	TArray<int32> Triangles;
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(3);

	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(0);

	TArray<FVector> normals;
	normals.Add(FVector(0, 0, 0));
	normals.Add(FVector(0, 0, 0));
	normals.Add(FVector(0, 0, 0));
	normals.Add(FVector(0, 0, 0));

	TArray<FVector2D> UV0;
	UV0.Add(FVector2D(0, 1));
	UV0.Add(FVector2D(1, 1));
	UV0.Add(FVector2D(1, 0));
	UV0.Add(FVector2D(0, 0));


	TArray<FColor> vertexColors;
	vertexColors.Add(FColor::Yellow);
	vertexColors.Add(FColor::Red);
	vertexColors.Add(FColor::Green);
	vertexColors.Add(FColor::Blue);


	TArray<FProcMeshTangent> tangents;
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));
	tangents.Add(FProcMeshTangent(1, 1, 1));


	mesh->CreateMeshSection(0, vertices, Triangles, normals, UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	mesh->SetMaterial(0, material);


	// With default options
	//mesh->CreateMeshSection(1, vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);

}

void AProceduralMesh::GeneratePiramid() {

}

void AProceduralMesh::GenerateCube(FVector Size) {
	// NOTE: Unreal uses an upper-left origin UV
	// NOTE: This sample uses a simple UV mapping scheme where each face is the same
	// NOTE: For a normal facing towards us, be build the polygon CCW in the order 0-1-2 then 0-2-3 to complete the quad.
	// Remember in Unreal, X is forwards, Y is to your right and Z is up.

	// Set up mesh buffers
	int32 vertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
	vertices.AddUninitialized(vertexCount);
	triangles.AddUninitialized(6 * 2 * 3); // 2x triangles per cube side, 3 verts each
	UV0s.AddUninitialized(vertexCount);
	normals.AddUninitialized(vertexCount);
	tangents.AddUninitialized(vertexCount);

	// Calculate a half offset so we get correct center of object
	float OffsetX = Size.X / 2.0f;
	float OffsetY = Size.Y / 2.0f;
	float OffsetZ = Size.Z / 2.0f;

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




	mesh->CreateMeshSection(3, vertices, triangles, normals, UV0s, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	mesh->SetMaterial(3, material);
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
	vertices[Index1] = bottomLeft;
	vertices[Index2] = bottomRight;
	vertices[Index3] = topRight;
	vertices[Index4] = topLeft;
	UV0s[Index1] = FVector2D(0.0f, 1.0f);
	UV0s[Index2] = FVector2D(1.0f, 1.0f);
	UV0s[Index3] = FVector2D(1.0f, 0.0f);
	UV0s[Index4] = FVector2D(0.0f, 0.0f);
	triangles[TriangleOffset++] = Index1;
	triangles[TriangleOffset++] = Index2;
	triangles[TriangleOffset++] = Index3;
	triangles[TriangleOffset++] = Index1;
	triangles[TriangleOffset++] = Index3;
	triangles[TriangleOffset++] = Index4;
	// On a cube side, all the vertex normals face the same way
	normals[Index1] = normals[Index2] = normals[Index3] = normals[Index4] = Normal;
	tangents[Index1] = tangents[Index2] = tangents[Index3] = tangents[Index4] = Tangent;
}




void AProceduralMesh::GenerateTube(int radius, int height, int circleSections, int heightSections) {
	// NOTE: Unreal uses an upper-left origin UV
	// NOTE: This sample uses a simple UV mapping scheme where each face is the same
	// NOTE: For a normal facing towards us, be build the polygon CCW in the order 0-1-2 then 0-2-3 to complete the quad.
	// Remember in Unreal, X is forwards, Y is to your right and Z is up.

	// Set up mesh buffers
	int32 vertexCount = circleSections * (heightSections + 1) * 2;
	vertices.AddUninitialized(vertexCount);
	triangles.AddUninitialized(circleSections * (heightSections + 1) * 3);
	UV0s.AddUninitialized(vertexCount);
	normals.AddUninitialized(vertexCount);
	tangents.AddUninitialized(vertexCount);

	// Now we create 6x faces, 4 vertices each
	int32 VertexOffset = 0;
	int32 TriangleOffset = 0;
	FVector normal = FVector();
	FProcMeshTangent tangent = FProcMeshTangent();



	// Make a cylinder section
	const float AngleBetweenQuads = (2.0f / (float)(circleSections)) * PI;
	const float VMapPerQuad = 1.0f / (float)circleSections;
	FVector Offset = FVector(0, 0, height);

	for (int32 QuadIndex = 0; QuadIndex < circleSections; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = FVector(FMath::Cos(Angle) * radius, FMath::Sin(Angle) * radius, 0.f);
		FVector p1 = FVector(FMath::Cos(NextAngle) * radius, FMath::Sin(NextAngle) * radius, 0.f);
		FVector p2 = p1 + Offset;
		FVector p3 = p0 + Offset;

		// Set up the quad triangles
		int32 VertIndex1 = VertexOffset++;
		int32 VertIndex2 = VertexOffset++;
		int32 VertIndex3 = VertexOffset++;
		int32 VertIndex4 = VertexOffset++;

		vertices[VertIndex1] = p0;
		vertices[VertIndex2] = p1;
		vertices[VertIndex3] = p2;
		vertices[VertIndex4] = p3;

		// Now create two triangles from those four vertices
		// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
		triangles[TriangleOffset++] = VertIndex4;
		triangles[TriangleOffset++] = VertIndex3;
		triangles[TriangleOffset++] = VertIndex1;

		triangles[TriangleOffset++] = VertIndex3;
		triangles[TriangleOffset++] = VertIndex2;
		triangles[TriangleOffset++] = VertIndex1;

		// UVs.  Note that Unreal UV origin (0,0) is top left
		UV0s[VertIndex1] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 1.0f);
		UV0s[VertIndex2] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 1.0f);
		UV0s[VertIndex3] = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 0.0f);
		UV0s[VertIndex4] = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 0.0f);

		// Normals
		FVector NormalCurrent = FVector::CrossProduct(vertices[VertIndex1] - vertices[VertIndex3], vertices[VertIndex2] - vertices[VertIndex3]).GetSafeNormal();

		bool bInSmoothNormals = true;

		if (bInSmoothNormals)
		{

			// To smooth normals we give the vertices different values than the polygon they belong to.
			// GPUs know how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			float NextNextAngle = (float)(QuadIndex + 2) * AngleBetweenQuads;
			FVector p4 = FVector(FMath::Cos(NextNextAngle) * radius, FMath::Sin(NextNextAngle) * radius, 0.f);

			// p1 to p4 to p2
			FVector NormalNext = FVector::CrossProduct(p1 - p2, p4 - p2).GetSafeNormal();
			FVector AverageNormalRight = (NormalCurrent + NormalNext) / 2;
			AverageNormalRight = AverageNormalRight.GetSafeNormal();

			float PreviousAngle = (float)(QuadIndex - 1) * AngleBetweenQuads;
			FVector pMinus1 = FVector(FMath::Cos(PreviousAngle) * radius, FMath::Sin(PreviousAngle) * radius, 0.f);

			// p0 to p3 to pMinus1
			FVector NormalPrevious = FVector::CrossProduct(p0 - pMinus1, p3 - pMinus1).GetSafeNormal();
			FVector AverageNormalLeft = (NormalCurrent + NormalPrevious) / 2;
			AverageNormalLeft = AverageNormalLeft.GetSafeNormal();

			normals[VertIndex1] = AverageNormalLeft;
			normals[VertIndex2] = AverageNormalRight;
			normals[VertIndex3] = AverageNormalRight;
			normals[VertIndex4] = AverageNormalLeft;
		}
		else
		{
			// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
			normals[VertIndex1] = normals[VertIndex2] = normals[VertIndex3] = normals[VertIndex4] = NormalCurrent;
		}

		// Tangents (perpendicular to the surface)
		FProcMeshTangent SurfaceTangent = FProcMeshTangent(1, 1, 1);
		tangents[VertIndex1] = tangents[VertIndex2] = tangents[VertIndex3] = tangents[VertIndex4] = SurfaceTangent;


	}


	mesh->CreateMeshSection(4, vertices, triangles, normals, UV0s, TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	mesh->SetMaterial(4, material);
}