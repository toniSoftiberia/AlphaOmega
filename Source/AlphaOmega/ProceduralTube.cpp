// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTube.h"



// We only need to call the method to build a sphere with the desired parameters
void AProceduralTube::GenerateMesh() {

	// Here we need to look for orientation of the tube
	FVector orientation = startPoint.GetLocation() - endPoint.GetLocation();

	BuildTube(startPoint.GetLocation(), endPoint.GetLocation(), startRotation, endRotation, startRadius, endRadius, circleSections, smoothNormals, useUniqueTexture, addStartCap, addEndCap);
}

