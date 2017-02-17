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
void AProceduralLandscape::GenerateHeights(int smoothStep) {

	FRandomStream RngStream = FRandomStream::FRandomStream(randomSeed);
	int32 i, j;

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



/** Generates the normal vale smoothed for each vertex based on generated heights*/
void AProceduralLandscape::PrecalculateSmoothNormals() {

	// Clean the values
	smoothValues.Empty();
	smoothValues.AddDefaulted(widthSections + 1);
	for (int k = 0; k < widthSections + 1; ++k)
		smoothValues[k].childs.AddDefaulted(lenghtSections + 1);

	// This vales will be used several times
	float widthStep = size.X / widthSections;
	float lengthStep = size.Y / lenghtSections;

	// Fill height data with random values
	for (int32 i = 0; i <= widthSections; ++i) {
		for (int32 j = 0; j <= lenghtSections; ++j) {

			// We use the top vertex for the first and we rotate clockwise
			// By default we use the vector direction for distance between vertex
			FVector v0 = FVector::ForwardVector * widthStep;
			if (i < widthSections)
				v0 = FVector(widthStep, 0, heightValues[i + 1][j]);

			FVector v1 = FVector::RightVector * lengthStep;
			if (j < lenghtSections)
				v1 = FVector(0, lengthStep, heightValues[i][j + 1]);

			FVector v2 = -FVector::ForwardVector * widthStep;
			if (i > 0)
				v2 = FVector(-widthStep, 0, heightValues[i - 1][j]);

			FVector v3 = -FVector::RightVector * lengthStep;
			if (j > 0)
				v3 = FVector(0, -lengthStep, heightValues[i][j - 1]);

			// Calculate the new normals
			FVector n0 = FVector::CrossProduct(v0, v1).GetSafeNormal();
			FVector n1 = FVector::CrossProduct(v1, v2).GetSafeNormal();
			FVector n2 = FVector::CrossProduct(v2, v3).GetSafeNormal();
			FVector n3 = FVector::CrossProduct(v3, v0).GetSafeNormal();

			smoothValues[i][j] = (n0 + n1 + n2 + n3) / 4;
		}
	}
}

/** With the heigth generated we need to calculate the blank positions with interpolation*/
void AProceduralLandscape::InterpolateTerrain(int smoothStep) {

	// Loop all terrain interpolating values
	for (int32 i = 0; i < widthSections + 1; i++) {
		for (int32 j = 0; j < lenghtSections + 1; j++) {

			// We need use diferent value of smooth step when we are in last positions, but we fill by default with smooth step
			int32 localSmoothStep = smoothStep;
			// First index to interpolate
			int32 backIndex = (j / smoothStep) * smoothStep;
			
			// If we are in last postions and length sections isn't multiple of smoothStep we need to reevaluate it
			while (backIndex + localSmoothStep > lenghtSections)
				--localSmoothStep;
			
			// Second index to interpolate
			int32 nextIndex = backIndex + localSmoothStep;
			// Position between next and back
			float interpolationPosition = (float)(j % smoothStep) / smoothStep;
			if (smoothInterpolation)
				interpolationPosition = FMath::SmoothStep(backIndex, nextIndex, j);
			// Assing the interpolated value to the array
			heightValues[i][j] = InterpolateVertex(heightValues[i][backIndex], heightValues[i][nextIndex], interpolationPosition);

			// If we are in a node we need to interpolate in X direction to
			if ((i % smoothStep == 0) && (j % smoothStep == 0)) {

				// We need use diferent value of smooth step when we are in last positions, but we fill by default with smooth step
				localSmoothStep = smoothStep;
				// If we are in last postions and length sections isn't multiple of smoothStep we need to reevaluate it
				while (i + localSmoothStep > widthSections)
					--localSmoothStep;
				
				// Loop in X direction
				for (int k = 1; k <= localSmoothStep; k++) {

					// Position between next and back, note that we allways use smoothStep here for better smoothing, if we use localSmoothStep it will go to 0 faster
					interpolationPosition = (float)k / smoothStep;
					if (smoothInterpolation)
						interpolationPosition = FMath::SmoothStep(i, i + smoothStep, i + k);

					// Assing the interpolated value to the array
					heightValues[i + k][j] = InterpolateVertex(heightValues[i][j], heightValues[i + localSmoothStep][j], interpolationPosition);
				}
			}
		}
	}
}

/** Makes the interpolation between a and b using alpha and the interpolation mode desired*/
float AProceduralLandscape::InterpolateVertex(const float a, const float b, const float alpha) {

	// Call the desired method to interpolate
	switch (interpolationMode) {

	case EInterpolationModes::IM_Circular_in:
		return FMath::InterpCircularIn(a, b, alpha);

	case EInterpolationModes::IM_Circular_out:
		return FMath::InterpCircularOut(a, b, alpha);

	case EInterpolationModes::IM_Circular_in_out:
		return FMath::InterpCircularInOut(a, b, alpha);

	case EInterpolationModes::IM_Ease_in:
		return FMath::InterpEaseIn(a, b, alpha, exponential);

	case EInterpolationModes::IM_Ease_out:
		return FMath::InterpEaseOut(a, b, alpha, exponential);

	case EInterpolationModes::IM_Ease_in_out:
		return FMath::InterpEaseInOut(a, b, alpha, exponential);

	case EInterpolationModes::IM_Expo_in:
		return FMath::InterpExpoIn(a, b, alpha);

	case EInterpolationModes::IM_Expo_out:
		return FMath::InterpExpoOut(a, b, alpha);

	case EInterpolationModes::IM_Expo_in_out:
		return FMath::InterpExpoInOut(a, b, alpha);

	case EInterpolationModes::IM_Sin_in:
		return FMath::InterpSinIn(a, b, alpha);

	case EInterpolationModes::IM_Sin_out:
		return FMath::InterpSinOut(a, b, alpha);

	case EInterpolationModes::IM_Sin_in_out:
		return FMath::InterpSinInOut(a, b, alpha);

	case EInterpolationModes::IM_Linear:
	default:
		return FMath::Lerp(a, b, alpha);
	}

}

/** Generates the height for the mesh using randomSeed*/
void AProceduralLandscape::GenerateSmoothTerrain(int smoothStep){
	
	// Generate the deired heights 
	GenerateHeights(smoothStep);

	// Interpolate the values of the calculated heights 
	InterpolateTerrain(smoothStep);

	// If required pprecalcule normals
	if (smoothNormals)
		PrecalculateSmoothNormals();
}

FVector AProceduralLandscape::GetSmoothFromIndex(int32 i, int32 j) {

	return smoothValues[i][j];
 }

