// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralQuad.h"


/** Sets up the values to generate the desired figure*/
void AProceduralQuad::GenerateMesh() {

	// We only need to call the method to build a quad with the desired parameters
	BuildQuad(bottomLeft, bottomRight, topRight, topLeft);
}
