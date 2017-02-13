// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralLandscape.h"


/** Sets up the values to generate the landscape*/
void AProceduralLandscape::GenerateMesh() {

	// Generate the height values
	GenerateHeights();

	// We only need to call the desired method to build the landscape
	BuildLandscape(size.X, size.Y, widthSections, lenghtSections, heightValues, smoothNormals, useUniqueTexture);
}


/** Generates the height for the mesh using randomSeed*/
void AProceduralLandscape::GenerateHeights(){

	FRandomStream RngStream = FRandomStream::FRandomStream(randomSeed);

	// Setup example height data
	int32 NumberOfPoints = (widthSections + 1) * (lenghtSections + 1);

	// Fill height data with random values
	int32 i,j;
	for (i = 0; i <= widthSections; i++) 
		for (j = 0; j <= lenghtSections; j++) {

			float randomValue = RngStream.FRandRange(0, size.Z);
			int32 heightIndex = (i*(lenghtSections)) + (j);
			heightValues[heightIndex] = randomValue;
		}

	int32 heightIndex = (i*(lenghtSections)) + (j);
}


FVector AProceduralLandscape::GetSmoothFromIndex(int32 i, int32 j) {
	FVector smooth = FVector::ZeroVector;
	FVector total = FVector::ZeroVector;
	//heightIndex = (i*(lenghtSections + 1)) + (j);
	FVector center = FVector(i, j, heightValues[(i*(lenghtSections + 1)) + (j)]);
	// We use the top vertex for the first and we rotate clockwise
	FVector p0 = FVector::ForwardVector * size.X / widthSections + center;
	if (i < lenghtSections) {
		if (i == 1 && j == 1)
		UE_LOG(LogClass, Log, TEXT("i < lenghtSections"));
		p0 = FVector(size.Y / lenghtSections, 0, heightValues[((i + 1) * (lenghtSections + 1)) + (j)]);
	}
	
	FVector p1 = FVector::RightVector * size.Y / lenghtSections  + center;
	if (j < widthSections) {
		if (i == 1 && j == 1)
		UE_LOG(LogClass, Log, TEXT("J < widthSections"));
		p1 = FVector(0 ,1 *size.X / widthSections, heightValues[(i* (lenghtSections + 1)) + (j + 1)]);
	}
	FVector p2 = -FVector::ForwardVector * size.X / widthSections + center;
	if (i > 0) {
		if (i == 1 && j == 1)
		UE_LOG(LogClass, Log, TEXT("i > 0"));
		p2 = FVector(-size.Y / lenghtSections, 0, heightValues[((i - 1) * (lenghtSections + 1)) + j]);
	}
	FVector p3 = -FVector::RightVector * size.Y / lenghtSections + center;
	if (j > 0) {
		if (i == 1 && j == 1)
		UE_LOG(LogClass, Log, TEXT("j > 0"));
		p3 = FVector(0, -1 * size.X / widthSections, heightValues[(i * (lenghtSections + 1)) + (j - 1)]);
	}

	FVector vn0 = p0;
	FVector vn1 = p1;
	FVector vn2 = p2;
	FVector vn3 = p3;

	FVector v0 = vn0.GetSafeNormal();
	FVector v1 = vn1.GetSafeNormal();
	FVector v2 = vn2.GetSafeNormal();
	FVector v3 = vn3.GetSafeNormal();


	FVector n0 = FVector::CrossProduct(v0, v1);
	FVector n1 = FVector::CrossProduct(v1, v2);
	FVector n2 = FVector::CrossProduct(v2, v3);
	FVector n3 = FVector::CrossProduct(v3, v0);
	FVector nn0 = FVector::CrossProduct(v0, v1).GetSafeNormal();
	FVector nn1 = FVector::CrossProduct(v1, v2).GetSafeNormal();
	FVector nn2 = FVector::CrossProduct(v2, v3).GetSafeNormal();
	FVector nn3 = FVector::CrossProduct(v3, v0).GetSafeNormal();

	FVector average = (n0 + n1 + n2 + n3) / 4;
	
	if (i == 1 && j == 1) {
		UE_LOG(LogClass, Log, TEXT("x: %d"), i);
		UE_LOG(LogClass, Log, TEXT("y: %d"), j);
		UE_LOG(LogClass, Log, TEXT("center: %s"), *center.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("z: %d"), (i*(lenghtSections + 1)) + (j));
		UE_LOG(LogClass, Log, TEXT("p0: %s"), *p0.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("p1: %s"), *p1.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("p2: %s"), *p2.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("p3: %s"), *p3.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("vn0: %s"), *vn0.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("vn1: %s"), *vn1.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("vn2: %s"), *vn2.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("vn3: %s"), *vn3.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("v0: %s"), *v0.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("v1: %s"), *v1.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("v2: %s"), *v2.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("v3: %s"), *v3.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n0: %s"), *n0.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n1: %s"), *n1.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n2: %s"), *n2.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n3: %s"), *n3.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n0: %s"), *nn0.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n1: %s"), *nn1.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n2: %s"), *nn2.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("n3: %s"), *nn3.ToCompactString());
		UE_LOG(LogClass, Log, TEXT("average: %s"), *average.ToCompactString());
	}

	return average;
 }

