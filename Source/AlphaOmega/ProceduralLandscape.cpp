// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralLandscape.h"


/** Sets up the values to generate the landscape*/
void AProceduralLandscape::GenerateMesh() {

	// Setup example height data
	int32 NumberOfPoints = (widthSections + 1) * (lenghtSections + 1);

	heightValues.Empty();
	heightValues.AddDefaulted(widthSections + 1);
	for (int k = 0; k < widthSections + 1; ++k)
		heightValues[k].childs.AddDefaulted(lenghtSections + 1);
	
		
	// Generate the height values
	GenerateSmoothTerrain(smoothStep);
	//GenerateHeights(1);

	// We only need to call the desired method to build the landscape
	BuildLandscape(size.X, size.Y, widthSections, lenghtSections, heightValues, smoothNormals, useUniqueTexture);
}


/** Generates the height for the mesh using randomSeed*/
void AProceduralLandscape::GenerateHeights(int smoothStep){

	FRandomStream RngStream = FRandomStream::FRandomStream(randomSeed);
	int32 i,j;

	// Fill height data with random values
	for (i = 0; i <= widthSections;) {
		for (j = 0; j <= lenghtSections;) {

			heightValues[i][j] = RngStream.FRandRange(0, size.Z);

			for (int k = 1; k < smoothStep; k++)
				RngStream.FRandRange(0, size.Z);

			j += smoothStep;
		}
		i += smoothStep;
	}
}


/** Generates the height for the mesh using randomSeed*/
void AProceduralLandscape::GenerateSmoothTerrain(int smoothStep){
	
	GenerateHeights(smoothStep);

	int32 i, j;
	int32 localSmoothStepJ = smoothStep;

	for (i = 0; i < widthSections + 1;i++) {
		UE_LOG(LogClass, Log, TEXT("i < widthSections + 1 -> %d < %d + 1 = %d"), i, widthSections, i < widthSections + 1);
		for (j = 0; j < lenghtSections + 1;j++) {
			UE_LOG(LogClass, Log, TEXT("j < lenghtSections + 1 -> %d < %d + 1 = %d"), j, lenghtSections, j < lenghtSections + 1);
			localSmoothStepJ = smoothStep;

			UE_LOG(LogClass, Log, TEXT("j %d"), j);
			UE_LOG(LogClass, Log, TEXT("i %d"), i);
			int32 backIndex = (j / smoothStep) * smoothStep;

			while (backIndex + localSmoothStepJ > lenghtSections) {
				--localSmoothStepJ;
			}
			UE_LOG(LogClass, Log, TEXT("localSmoothStepJ %d"), localSmoothStepJ);
						
				int32 localSmoothStepI = smoothStep;
						
				while (i + localSmoothStepI > widthSections) {
					--localSmoothStepI;
				}
			UE_LOG(LogClass, Log, TEXT("j backIndex %d"), backIndex);

			int32 nextIndex = ((j / smoothStep) * (smoothStep)) + localSmoothStepJ;
			UE_LOG(LogClass, Log, TEXT("j nextIndex %d"), nextIndex);

			float interpolationPosition = (float)(j % smoothStep) / smoothStep;
			UE_LOG(LogClass, Log, TEXT("interpolationPosition %f"), interpolationPosition);	

			float interpolatedValue = FMath::Lerp(heightValues[i][backIndex], heightValues[i][nextIndex], interpolationPosition);
			UE_LOG(LogClass, Log, TEXT("interpolatedValue %f"), interpolatedValue);

			heightValues[i][j] = interpolatedValue;
			UE_LOG(LogClass, Log, TEXT("heightValues[%d][%d] %f"), i, j, heightValues[i][j]);


			if ((i % smoothStep == 0) && (j % smoothStep == 0)) {


				for (int k = 1; k < localSmoothStepI; k++) {

						
						UE_LOG(LogClass, Log, TEXT("i localSmoothStep %d"), localSmoothStepI);
						UE_LOG(LogClass, Log, TEXT("i backIndex %d"), i);
						UE_LOG(LogClass, Log, TEXT("i nextIndex %d"), i + localSmoothStepI);

						float interpolationPosition = (float)k / localSmoothStepI;

						UE_LOG(LogClass, Log, TEXT("i interpolationPosition %f"), interpolationPosition);
						if (localSmoothStepI == 0)
							localSmoothStepI = 1.f;

						interpolatedValue = FMath::Lerp(heightValues[i][j], heightValues[i+ localSmoothStepI][j], interpolationPosition);
						
						UE_LOG(LogClass, Log, TEXT("interpolatedValue %f"), interpolatedValue);

						heightValues[i + k -(smoothStep - localSmoothStepI)][j] = interpolatedValue;

						UE_LOG(LogClass, Log, TEXT("heightValues[%d][%d] %f"), i + k - (smoothStep - localSmoothStepI), j, heightValues[i + k - (smoothStep - localSmoothStepI)][j]);
			
								
				}
			}
		}
	}
}

FVector AProceduralLandscape::GetSmoothFromIndex(int32 i, int32 j) {
	
	// We use the top vertex for the first and we rotate clockwise
	// By default we use the vector direction for distance between vertex
	FVector v0 = FVector::ForwardVector * size.X / widthSections;
	if (i < widthSections)
		v0 = FVector(size.X / widthSections, 0, heightValues[i + 1][j]);
	
	FVector v1 = FVector::RightVector * size.Y / lenghtSections;
	if (j < lenghtSections)
		v1 = FVector(0 ,size.Y / lenghtSections, heightValues[i][j + 1]);
	
	FVector v2 = -FVector::ForwardVector * size.X / widthSections;
	if (i > 0) 
		v2 = FVector(-size.X / widthSections, 0, heightValues[i - 1][j]);
	
	FVector v3 = -FVector::RightVector * size.Y / lenghtSections;
	if (j > 0) 
		v3 = FVector(0, -size.Y / lenghtSections, heightValues[i][j - 1]);

	// Calculate the new normals
	FVector n0 = FVector::CrossProduct(v0, v1).GetSafeNormal();
	FVector n1 = FVector::CrossProduct(v1, v2).GetSafeNormal();
	FVector n2 = FVector::CrossProduct(v2, v3).GetSafeNormal();
	FVector n3 = FVector::CrossProduct(v3, v0).GetSafeNormal();

	return (n0 + n1 + n2 + n3) / 4;
 }

