// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTube.h"



// We only need to call the method to build a sphere with the desired parameters
void AProceduralTube::GenerateMesh() {

	// Here we need to look for orientation of the tube
	FVector orientationStart = (startPoint.GetLocation() - endPoint.GetLocation()).GetSafeNormal();
	FVector orientationEnd = (endPoint.GetLocation() - startPoint.GetLocation()).GetSafeNormal();

	BuildTubeFromOrientation(startPoint.GetLocation(), endPoint.GetLocation(), startRadius, circleSections, smoothNormals, useUniqueTexture, addStartCap, addEndCap);
}

