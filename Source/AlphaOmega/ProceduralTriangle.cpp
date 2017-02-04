// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTriangle.h"

/** Sets up the values to generate the desired figure*/
void AProceduralTriangle::GenerateMesh() {

	// We only need to call the method to build a sphere with the desired parameters
	if(verticesLocation.Num() == 3)
		BuildTriangle(verticesLocation[0].GetLocation(), verticesLocation[1].GetLocation(), verticesLocation[2].GetLocation());

}

