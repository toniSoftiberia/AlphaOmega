// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralQuad.h"


/** Sets up the values to generate the desired figure*/
void AProceduralQuad::GenerateMesh() {

	// We only need to call the method to build a quad with the desired parameters
	if (verticesLocation.Num() == 4)
		BuildQuad(verticesLocation[0].GetLocation(), verticesLocation[1].GetLocation(), verticesLocation[2].GetLocation(), verticesLocation[3].GetLocation());
}
