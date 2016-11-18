// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ProceduralMesh.generated.h"

UCLASS()
class ALPHAOMEGA_API AProceduralMesh : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProceduralMesh();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void GenerateTriangle();

	void GenerateQuad();

	void GeneratePiramid();

	void GenerateCube(FVector Size);

	void GenerateTube(int radius, int height, int heightSections, int circleSections);

	/*
	void BuildQuad(
	TArray<FVector>& Vertices,
	TArray<int32>& Triangles,
	FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft,
	int32& VertexOffset,
	int32& TriangleOffset,
	FPackedNormal Normal,
	FPackedNormal Tangent);
	*/

	void BuildQuad(
		FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft,
		int32& VertexOffset,
		int32& TriangleOffset,
		FVector& Normal,
		FProcMeshTangent& Tangent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		UMaterialInterface* material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		UProceduralMeshComponent* mesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<FVector> vertices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<int32> triangles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<FVector> normals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<FVector2D> UV0s;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		TArray<FProcMeshTangent> tangents;
};
