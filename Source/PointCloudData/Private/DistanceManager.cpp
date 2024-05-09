// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceManager.h"

UDistanceManager::UDistanceManager()
{
	ElementID = 0;
	DistanceItems = TArray<DistanceItem*>();
}

UDistanceManager::~UDistanceManager()
{
}

void UDistanceManager::AddActor(AActor* Actor,FVector CorrectLocation)
{
	ElementID ++;
	DistanceItem* Item = new DistanceItem(Actor,ElementID,CorrectLocation);
	DistanceItems.Add(Item);
}

void UDistanceManager::ResetList()
{
	DistanceItems.Empty();
	ElementID = 0;
}

int UDistanceManager::GetNumberOfActors()
{
	return DistanceItems.Num();
}

TArray<AActor*> UDistanceManager::GetActorsFromList()
{
	TArray<AActor*> Actors;
	
	for(DistanceItem* Item: DistanceItems)
	{
		Actors.Add(Item->getActor());
	}
	return Actors;
}


TArray<FActorDistanceTuple> UDistanceManager::CalculateDistances()
{
	TArray<FActorDistanceTuple> Result;

	for (int i = 0; i < DistanceItems.Num(); i++)
	{
		for (int j = i + 1; j < DistanceItems.Num(); j++)
		{
			FActorDistanceTuple Tuple;
			Tuple.ID1 = DistanceItems[i]->getID();
			Tuple.ID2 = DistanceItems[j]->getID();

			FVector Location1 = DistanceItems[i]->getLocation();
			FVector Location2 = DistanceItems[j]->getLocation();
			
			FVector DistanceVector = Location2 - Location1;

			Tuple.Distance = DistanceVector;

			Result.Add(Tuple);
		}
	}

	return Result;
}

TArray<AActor*> UDistanceManager::GetActorsFromTuple(const FActorDistanceTuple& Tuple)
{
	return GetActorsFromIDs(Tuple.ID1, Tuple.ID2);
}

FVector UDistanceManager::GetDistanceFromTuple(const FActorDistanceTuple& Tuple)
{
	return Tuple.Distance;
}

TArray<FVector> UDistanceManager::GetCorrectLocationsFromTuple(const FActorDistanceTuple& Tuple)
{
	return GetLocationsFromIDs(Tuple.ID1, Tuple.ID2);
}

TArray<AActor*> UDistanceManager::GetActorsFromIDs(int64 ID1, int64 ID2)
{
	TArray<AActor*> Result;

	for (DistanceItem* Item : DistanceItems)
	{
		if (Item->getID() == ID1 || Item->getID() == ID2)
		{
			Result.Add(Item->getActor());
			
			if (Result.Num() == 2)
			{
				break;
			}
		}
	}

	return Result;
}

TArray<FVector> UDistanceManager::GetLocationsFromIDs(int64 ID1, int64 ID2)
{
	TArray<FVector> Result;

	for (DistanceItem* Item : DistanceItems)
	{
		if (Item->getID() == ID1 || Item->getID() == ID2)
		{
			Result.Add(Item->getLocation());
			
			if (Result.Num() == 2)
			{
				break;
			}
		}
	}
	return Result;
}
