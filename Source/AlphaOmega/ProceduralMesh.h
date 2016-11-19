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

	void BuildTriangle(
		FVector vertexA, FVector vertexB, FVector vertexC,
		int32& VertexOffset,
		int32& TriangleOffset,
		FVector& normal,
		FProcMeshTangent& tangent);

	void BuildQuad(
		FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft,
		int32& VertexOffset,
		int32& TriangleOffset,
		FVector& Normal,
		FProcMeshTangent& Tangent);

	virtual void GenerateMesh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		UMaterialInterface* material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		UProceduralMeshComponent* mesh;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> vertices;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<int32> triangles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> normals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector2D> UV0s;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FProcMeshTangent> tangents;
};