// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "ProceduralUtils.h"
#include "GameFramework/Actor.h"
#include "ProceduralMesh.generated.h"

/**
* Base class for all the procedural figures
*/
UCLASS()
class ALPHAOMEGA_API AProceduralMesh : public AActor
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame (disabled)
	virtual void Tick(float DeltaSeconds) override;

#if WITH_EDITOR

	/** Creates the base componentes for the mesh and cleans all sections*/
	AProceduralMesh();

	/** Used for editing meshes on realtime*/
	//void OnConstruction(const FTransform& Transform) override;

#endif

	/** Array of FVectors to store the location of the vertices*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> vertices;

	/** Array of FVectors2D to store the UV0 values, must have the same length as vertices*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector2D> UV0s;

	/** Array of FVectors to store the normal of each vertex, must have the same length as vertices*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> normals;

	/** Array of FProcMeshTangent to store the tangent of each vertex, must have the same length as vertices*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FProcMeshTangent> tangents;

	/** Array to indicate what vertices define each triangle, every 3 vertices are a triangle*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<int32> triangles;

	/** Used to know what will be the next triangle*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		int32 vertexOffset;

	/** The mesh component to generate the mesh*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		UProceduralMeshComponent* mesh;


public:

	/** Generates the mesh that we have in our local variables*/
	virtual void GenerateMesh();

	/** Generates a triangle from input vertices*/
	void BuildTriangle(	FVector vertexA, FVector vertexB, FVector vertexC,
		FVector& normal, FProcMeshTangent& tangent);

	/** Generates a quad from input vertices*/
	void BuildQuad(	FVector bottomLeft, FVector bottomRight, FVector topRight, FVector topLeft,
		FVector& normal, FProcMeshTangent& tangent);

	/** Generates a cube of cubesize size*/
	void BuildCube(	FVector cubeSize,
		FProcMeshTangent& tangent);

	/** Generates a piramid from input values*/
	void BuildPiramid( float height, float radius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addBottomCap,
		FProcMeshTangent& tangent);

	/** Generates a sphere from input values*/
	void BuildSphere(FVector center, float radius, int32 circleSections, int32 heightSections, bool smoothNormals, bool useUniqueTexture,
		FProcMeshTangent& tangent);

	/** Generates a tube from input values*/
	void BuildTube(FVector startPoint, FVector endPoint, FVector startRotation, FVector endRotation, float startRadius, float endRadius, int32 circleSections, bool smoothNormals, bool useUniqueTexture, bool addCaps,
		FProcMeshTangent& tangent);

	/** The material to use in this mesh*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
		UMaterialInterface* material;
};
