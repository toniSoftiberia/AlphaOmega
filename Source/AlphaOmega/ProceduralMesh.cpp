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

FVector AProceduralMesh::RotatePointAroundPivot(FVector InPoint, FVector InPivot, FVector InAngles)
{
	FVector direction = InPoint - InPivot; // get point direction relative to pivot
	direction = FQuat::MakeFromEuler(InAngles) * direction; // rotate it
	return direction + InPivot; // calculate rotated point
}
