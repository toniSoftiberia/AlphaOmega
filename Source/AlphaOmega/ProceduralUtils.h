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
struct FProceduralVertex{

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
	FProceduralVertex(const FVector& newPosition, const FVector2D& newUV0, const FVector& newNormal, const FProcMeshTangent& newTangent){
		position = newPosition;
		UV0 = newUV0;
		normal = newNormal;
		tangent = newTangent;
	}

	/** base constructor*/
	FProceduralVertex(){
		position = FVector::ZeroVector;
		UV0 = FVector2D::ZeroVector;
		normal = FVector::ZeroVector;
		tangent = FProcMeshTangent(0.f, 0.f, 0.f);
	}
};


/**
* Two dimensional array of floats
*/
USTRUCT()
struct FFloatArray{

	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<float> childs;

	float& operator [](int idx) {
		return childs[idx];
	}

	float operator [](int idx) const {
		return childs[idx];
	}
};


/**
* Two dimensional array of FVector
*/
USTRUCT()
struct FVectorArray{

	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procedural Parameters")
		TArray<FVector> childs;

	FVector& operator [](int idx) {
		return childs[idx];
	}

	FVector operator [](int idx) const {
		return childs[idx];
	}
};


/**
* Enum to identify the different types of interpolation we have to add BlueprintType to allow use it on editor
*/
UENUM(BlueprintType)
enum class EInterpolationModes : uint8 {

	IM_Circular_in 	UMETA(DisplayName = "Circular In"), // InterpCircularIn
	IM_Circular_out 	UMETA(DisplayName = "Circular Out"), // InterpCircularOut
	IM_Circular_in_out 	UMETA(DisplayName = "Circular In/Out"), // InterpCircularInOut

	IM_Ease_in 	UMETA(DisplayName = "Ease In"), // InterpEaseIn
	IM_Ease_out 	UMETA(DisplayName = "Ease Out"), // InterpEaseOut
	IM_Ease_in_out 	UMETA(DisplayName = "Ease In/Out"), // InterpEaseInOut

	IM_Expo_in 	UMETA(DisplayName = "Expo In"), // InterpExpoIn
	IM_Expo_out 	UMETA(DisplayName = "Expo Out"), // InterpExpoOut
	IM_Expo_in_out 	UMETA(DisplayName = "Expo In/Out"), // InterpExpoInOut

	IM_Sin_in 	UMETA(DisplayName = "Sin In"), // InterpSinIn
	IM_Sin_out 	UMETA(DisplayName = "Sin Out"), // InterpSinOut
	IM_Sin_in_out 	UMETA(DisplayName = "Sin In/Out"), // InterpSinInOut

	IM_Linear UMETA(DisplayName = "Linear")
};


/**
 * Here we allocate the static common functions for the project
 */
UCLASS()
class ALPHAOMEGA_API UProceduralUtils : public UObject{

	GENERATED_BODY()
	
public:

	static FVector RotatePointAroundPivot(FVector inPoint, FVector inPivot, FVector inAngles);

	
	
};
