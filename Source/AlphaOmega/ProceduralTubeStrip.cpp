// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphaOmega.h"
#include "ProceduralTubeStrip.h"



void AProceduralTubeStrip::GenerateMesh() {

	if (sstripPoints.Num() > 2) {
		
		FVector orientationBack = sstripPoints[0]->GetActorLocation() - sstripPoints[sstripPoints.Num()-1]->GetActorLocation();
		FVector orientationForward = sstripPoints[0]->GetActorLocation() - sstripPoints[1]->GetActorLocation();
		FVector rotation0 = orientationBack.GetSafeNormal();
		FVector rotationEnd = rotation0;
		FVector rotationStart;

		if (isClosedStrip){ 
			rotation0 = rotationEnd = (orientationBack.GetSafeNormal() - orientationForward.GetSafeNormal()).GetSafeNormal();
		}

		bool startCap;
		bool endCap;

		for (int i = 1; i < sstripPoints.Num(); ++i) {

			startCap = i == 1 && addCaps;
			endCap = i == sstripPoints.Num() - 1 && addCaps;

			int nextIndex = i < sstripPoints.Num() - 1 ? i + 1 : 0;

			rotationStart = rotationEnd;

			orientationBack = sstripPoints[i]->GetActorLocation() - sstripPoints[i-1]->GetActorLocation();
			orientationForward = sstripPoints[i]->GetActorLocation() - sstripPoints[nextIndex]->GetActorLocation();

			rotationEnd = (orientationBack.GetSafeNormal() - orientationForward.GetSafeNormal()).GetSafeNormal();

			if (i == 1 && !isClosedStrip)
				rotationStart = orientationBack.GetSafeNormal();
			if (nextIndex == 0 && !isClosedStrip)
				rotationEnd = orientationBack.GetSafeNormal();

			BuildTube(sstripPoints[i-1]->GetActorLocation(), sstripPoints[i]->GetActorLocation(), rotationStart, rotationEnd, sstripPoints[i-1]->GetActorScale().X, sstripPoints[i]->GetActorScale().X, circleSections, smoothNormals, useUniqueTexture, startCap, endCap);
		
		}

		if (isClosedStrip) {

			BuildTube(sstripPoints[sstripPoints.Num() - 1]->GetActorLocation(), sstripPoints[0]->GetActorLocation(), rotationEnd, rotation0, sstripPoints[sstripPoints.Num() - 1]->GetActorScale().X, sstripPoints[0]->GetActorScale().X, circleSections, smoothNormals, useUniqueTexture, false, false);
		}
		
	}
}

