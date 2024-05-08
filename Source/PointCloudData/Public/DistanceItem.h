// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class POINTCLOUDDATA_API DistanceItem
{
public:
	DistanceItem();
	DistanceItem(AActor* Target, int64 TargetID, FVector Location);
	AActor* getActor();
	int64 getID();
	FVector getLocation();
	~DistanceItem();
private:
	AActor* Actor;
	int64 ID;
	FVector CorrectLocation;
};
