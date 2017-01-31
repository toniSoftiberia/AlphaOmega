// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ProceduralMesh.generated.h"

UCLASS()
class ALPHAOMEGA_API AProceduralMesh : public AActor
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR
	//void OnConstruction(const FTransform& Transform) override;
#endif   // WITH_EDITOR

	// Sets default values for this actor's properties
	AProceduralMesh();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> vertices;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FProcMeshVertex> verticesComplete;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<int32> triangles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> normals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector2D> UV0s;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FProcMeshTangent> tangents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		UProceduralMeshComponent* mesh;

public:

	virtual void GenerateMesh();

	FVector RotatePointAroundPivot(FVector inPoint, FVector inPivot, FVector inAngles);

	void BuildTriangle(	FVector vertexA, FVector vertexB, FVector vertexC,
		int32& vertexOffset, int32& triangleOffset, FVector& normal, FProcMeshTangent& tangent);

	void BuildQuad(	FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft,
		int32& vertexOffset, int32& triangleOffset, FVector& normal, FProcMeshTangent& tangent);

	void BuildCube(	FVector cubeSize,
		int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent);

	void BuildPiramid( float height, float radius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addBottomCap,
		int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent);

	void BuildSphere(FVector center, float radius, int32 circleSections, int32 heightSections, bool smoothNormals, bool useUniqueTexture,
		int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent);

	void BuildTube(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addCaps,
		int32& vertexOffset, int32& triangleOffset, FProcMeshTangent& tangent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		UMaterialInterface* material;
};
