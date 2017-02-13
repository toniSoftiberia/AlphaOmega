// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralSphere.h"


/** Sets up the values to generate the sphere*/
void AProceduralSphere::GenerateMesh() {

	// We only need to call the desired method to build a sphere
	if(bGeodesic)
		BuildGeodesicSphere(center, radius, depth, smoothNormals);
	else
		BuildUVSphere(center, radius, circleSections, heightSections, smoothNormals, useUniqueTexture);
}



