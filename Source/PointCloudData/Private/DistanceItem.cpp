// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceItem.h"

DistanceItem::DistanceItem()
{
}

DistanceItem::DistanceItem(AActor* Target, int64 TargetID, FVector Location)
	: Actor(Target), ID(TargetID), CorrectLocation(Location)
{
}

AActor* DistanceItem::getActor()
{
	return Actor;
}

int64 DistanceItem::getID()
{
	return ID;
}

FVector DistanceItem::getLocation()
{
	return CorrectLocation;
}

DistanceItem::~DistanceItem()
{
}
