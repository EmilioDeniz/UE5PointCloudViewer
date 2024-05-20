// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceItem.h"

DistanceItem::DistanceItem()
{
}

DistanceItem::DistanceItem(AActor* Target, int64 TargetID, FVector Location,FVector ReferencePoint)
	: Actor(Target), ID(TargetID), CorrectLocation(Location), ReferenceLocation(ReferencePoint)
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

void DistanceItem::setLocation(FVector NewLocation)
{
	CorrectLocation = NewLocation;
}

FVector DistanceItem::getReferenceLocation()
{
	return ReferenceLocation;
}


DistanceItem::~DistanceItem()
{
}
