// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"


void AProceduralTubeStrip::GenerateMesh() {

	if (stripPoints.Num() > 2) {
		
		FTransform test;

		FVector orientationBack = stripPoints[0].GetLocation() - stripPoints[stripPoints.Num()-1].GetLocation();
		FVector orientationForward = stripPoints[0].GetLocation() - stripPoints[1].GetLocation();
		FVector rotation0 = orientationBack.GetSafeNormal();
		FVector rotationEnd = rotation0;
		FVector rotationStart;

		if (isClosedStrip){ 
			rotation0 = rotationEnd = (orientationBack.GetSafeNormal() - orientationForward.GetSafeNormal()).GetSafeNormal();
		}

		bool startCap;
		bool endCap;

		for (int i = 1; i < stripPoints.Num(); ++i) {

			UE_LOG(LogClass, Log, TEXT("stripPoints[i].GetLocation(): %s"), *(stripPoints[i].GetLocation()).ToString());

			startCap = i == 1 && addCaps;
			endCap = i == stripPoints.Num() - 1 && addCaps;

			int nextIndex = i < stripPoints.Num() - 1 ? i + 1 : 0;

			rotationStart = rotationEnd;

			orientationBack = stripPoints[i].GetLocation() - stripPoints[i-1].GetLocation();
			orientationForward = stripPoints[i].GetLocation() - stripPoints[nextIndex].GetLocation();

			rotationEnd = (orientationBack.GetSafeNormal() - orientationForward.GetSafeNormal()).GetSafeNormal();

			if (i == 1 && !isClosedStrip)
				rotationStart = FVector::ZeroVector;
			if (nextIndex == 0 && !isClosedStrip)
				rotationEnd = FVector::ZeroVector;

			BuildTube(stripPoints[i - 1].GetLocation(), stripPoints[i].GetLocation(), rotationStart, rotationEnd, stripPoints[i - 1].GetScale3D().X, stripPoints[i].GetScale3D().X, circleSections, smoothNormals, useUniqueTexture, startCap, endCap);
			
		}

		if (isClosedStrip) {

			BuildTube(stripPoints[stripPoints.Num() - 1].GetLocation(), stripPoints[0].GetLocation(), rotationEnd, rotation0, stripPoints[stripPoints.Num() - 1].GetScale3D().X, stripPoints[0].GetScale3D().X, circleSections, smoothNormals, useUniqueTexture, false, false);
		}
		
	}
}

