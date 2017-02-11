// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"


void AProceduralTubeStrip::GenerateMesh() {

	bool startCap = false;
	bool endCap = false;

	if (stripPoints.Num() > 2) {
		
		FTransform test;

		FVector orientationBack = stripPoints[1].GetLocation() - stripPoints[0].GetLocation();
		FVector orientationForward = stripPoints[1].GetLocation() - stripPoints[2].GetLocation();

		FVector orientationTube = stripPoints[0].GetLocation() - stripPoints[1].GetLocation();

		FVector orientationAverage = (orientationBack.GetSafeNormal() + orientationForward.GetSafeNormal()) / 2;

		if ((orientationForward.X - orientationTube.X) < 0 || (orientationForward.Y - orientationTube.Y) < 0) {
			orientationAverage *= -1;
			orientationAverage.Normalize();
		}

		FQuat rotationEnd = orientationAverage.ToOrientationQuat();


		FQuat rotationStart = stripPoints[0].GetRotation();
		rotationStart.Z = rotationEnd.Z;
		rotationStart.W = rotationEnd.W;
		rotationStart.Normalize();
		
		stripPoints[0].SetRotation(rotationStart);
		stripPoints[1].SetRotation(rotationEnd);
			
		BuildTubeFromRotations(
				stripPoints[0].GetLocation(), stripPoints[1].GetLocation(), 
				stripPoints[0].GetRotation().Euler(), stripPoints[1].GetRotation().Euler(),
				stripPoints[0].GetScale3D().X, stripPoints[1].GetScale3D().X, 
				circleSections, smoothNormals, useUniqueTexture, startCap, endCap);
		
	}
}

