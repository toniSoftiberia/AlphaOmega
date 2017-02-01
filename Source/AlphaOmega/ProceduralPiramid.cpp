// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralPiramid.h"

/** Sets up the values to generate the desired figure*/
void AProceduralPiramid::GenerateMesh() {

	// We only need to call the method to build a piramid with the desired parameters
	BuildPiramid(height,  radius, circleSections, smoothNormals, useUniqueTexture, addBottomCap);
}



