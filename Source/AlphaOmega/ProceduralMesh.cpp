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


