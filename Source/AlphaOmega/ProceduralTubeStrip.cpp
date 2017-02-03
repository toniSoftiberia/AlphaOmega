// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"



void AProceduralTubeStrip::GenerateMesh() {

	if (stripPoints.Num() > 2) {
		
		FVector orientationBack = stripPoints[0].position - stripPoints[stripPoints.Num()-1].position;
		FVector orientationForward = stripPoints[0].position - stripPoints[1].position;
		FVector rotation0 = orientationBack.GetSafeNormal();
		FVector rotationEnd = rotation0;
		FVector rotationStart;

		if (isClosedStrip){ 
			rotation0 = rotationEnd = (orientationBack.GetSafeNormal() - orientationForward.GetSafeNormal()).GetSafeNormal();
		}

		bool startCap;
		bool endCap;

		for (int i = 1; i < stripPoints.Num(); ++i) {

			startCap = i == 1 && addCaps;
			endCap = i == stripPoints.Num() - 1 && addCaps;

			int nextIndex = i < stripPoints.Num() - 1 ? i + 1 : 0;

			rotationStart = rotationEnd;

			orientationBack = stripPoints[i].position - stripPoints[i-1].position;
			orientationForward = stripPoints[i].position - stripPoints[nextIndex].position;

			rotationEnd = (orientationBack.GetSafeNormal() - orientationForward.GetSafeNormal()).GetSafeNormal();

			if (i == 1 && !isClosedStrip)
				rotationStart = orientationBack.GetSafeNormal();
			if (nextIndex == 0 && !isClosedStrip)
				rotationEnd = orientationBack.GetSafeNormal();

			BuildTube(stripPoints[i-1].position, stripPoints[i].position, rotationStart, rotationEnd, stripPoints[i-1].radius, stripPoints[i].radius, circleSections, smoothNormals, useUniqueTexture, startCap, endCap);
		
		}

		if (isClosedStrip) {

			BuildTube(stripPoints[stripPoints.Num() - 1].position, stripPoints[0].position, rotationEnd, rotation0, stripPoints[stripPoints.Num() - 1].radius, stripPoints[0].radius, circleSections, smoothNormals, useUniqueTexture, false, false);
		}
		
	}
}

