// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"



void AProceduralTubeStrip::GenerateMesh() {

	if (stripPoints.Num() > 2) {

		FVector vectorA = stripPoints[1].position - stripPoints[0].position;
		FVector vectorB = stripPoints[1].position - stripPoints[2].position;
		FVector rotationA = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

		vectorA = stripPoints[2].position - stripPoints[1].position;
		vectorB = stripPoints[2].position - stripPoints[3].position;
		FVector rotationB = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

		vectorA = stripPoints[3].position - stripPoints[2].position;
		vectorB = stripPoints[3].position - stripPoints[0].position;
		FVector rotationC = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

		vectorA = stripPoints[0].position - stripPoints[3].position;
		vectorB = stripPoints[0].position - stripPoints[1].position;
		FVector rotationD = (vectorA.GetSafeNormal() - vectorB.GetSafeNormal()).GetSafeNormal();

		BuildTube(stripPoints[0].position, stripPoints[1].position, rotationD, rotationA, stripPoints[0].radius, stripPoints[1].radius, circleSections, smoothNormals, useUniqueTexture, addCaps);
		BuildTube(stripPoints[1].position, stripPoints[2].position, rotationA, rotationB, stripPoints[1].radius, stripPoints[2].radius, circleSections, smoothNormals, useUniqueTexture, addCaps);
		BuildTube(stripPoints[2].position, stripPoints[3].position, rotationB, rotationC, stripPoints[2].radius, stripPoints[3].radius, circleSections, smoothNormals, useUniqueTexture, addCaps);
		BuildTube(stripPoints[3].position, stripPoints[0].position, rotationC, rotationD, stripPoints[3].radius, stripPoints[0].radius, circleSections, smoothNormals, useUniqueTexture, addCaps);
	}
}

