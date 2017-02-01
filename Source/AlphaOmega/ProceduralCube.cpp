// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralCube.h"



/** Sets up the values to generate the desired figure*/
void AProceduralCube::GenerateMesh() {

	// We only need to call the method to build a cube with the desired parameters
	BuildCube(cubeSize);
}

