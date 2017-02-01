// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralSphere.h"


/** Sets up the values to generate the desired figure*/
void AProceduralSphere::GenerateMesh() {

	// We only need to call the method to build a sphere with the desired parameters
	BuildSphere(center, radius, circleSections, heightSections, smoothNormals, useUniqueTexture);
}



