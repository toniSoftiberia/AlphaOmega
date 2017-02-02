// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
#include "Object.h"
#include "ProceduralUtils.generated.h"


//  because this is defined in your static library that all your classes depend on,
//  you can use this USTRUCT() data type in any class, even in  .h FORCEINLINE functions!

/**
* Store the different variables that make up a vertex
*/
USTRUCT()
struct FProceduralVertex
{
	GENERATED_USTRUCT_BODY()		

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		FVector position;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		FVector2D UV0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		FVector normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		FProcMeshTangent tangent;

	/** Overloaded constructor*/
	FProceduralVertex(const FVector& newPosition, const FVector2D& newUV0, const FVector& newNormal, const FProcMeshTangent& newTangent)
	{
		position = newPosition;
		UV0 = newUV0;
		normal = newNormal;
		tangent = newTangent;
	}

	/** base constructor*/
	FProceduralVertex()
	{
		position = FVector::ZeroVector;
		UV0 = FVector2D::ZeroVector;
		normal = FVector::ZeroVector;
		tangent = FProcMeshTangent(0.f, 0.f, 0.f);
	}
};

/**
 * Here we allocate the static common functions for the project
 */
UCLASS()
class ALPHAOMEGA_API UProceduralUtils : public UObject
{
	GENERATED_BODY()
	
public:

	static FVector UProceduralUtils::RotatePointAroundPivot(FVector inPoint, FVector inPivot, FVector inAngles);
	
	
};
