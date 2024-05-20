// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DistanceItem.h"
#include "Containers/Array.h" 
#include "DistanceManager.generated.h"

USTRUCT(BlueprintType)
struct FActorDistanceTuple
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int64 ID1;

	UPROPERTY(BlueprintReadWrite)
	int64 ID2;

	UPROPERTY(BlueprintReadWrite)
	FVector Distance;
};

UCLASS(Blueprintable)
class POINTCLOUDDATA_API UDistanceManager : public UObject
{
	GENERATED_BODY()
public:
	UDistanceManager();
	~UDistanceManager();
	
	UFUNCTION(BlueprintCallable, Category = "Distance Manager") 
	void AddActor(AActor* Actor,FVector CorrectLocation,FVector PointLocation);

	UFUNCTION(BlueprintCallable, Category = "Distance Manager") 
	void ResetList();
	
	UFUNCTION(BlueprintCallable, Category = "Distance Manager") 
	TArray<AActor*> GetActorsFromList();
	
	UFUNCTION(BlueprintCallable, Category = "Distance Manager") 
	int GetNumberOfActors();

	UFUNCTION(BlueprintCallable, Category = "Distance Manager")
	TArray<FActorDistanceTuple> CalculateDistances();
	
	UFUNCTION(BlueprintCallable, Category = "Distance Manager")
	TArray<AActor*> GetActorsFromTuple(const FActorDistanceTuple& Tuple);

	UFUNCTION(BlueprintCallable, Category = "Distance Manager")
	FVector GetDistanceFromTuple(const FActorDistanceTuple& Tuple);

	UFUNCTION(BlueprintCallable, Category = "Distance Manager")
	TArray<FVector> GetCorrectLocationsFromTuple(const FActorDistanceTuple& Tuple);

	UFUNCTION(BlueprintCallable, Category = "Distance Manager")
	void UpdateCorrectLocationOfActor(AActor* Actor, FTransform Transform);

private:
	int ElementID;
	TArray<DistanceItem*> DistanceItems;
	TArray<AActor*> GetActorsFromIDs(int64 ID1, int64 ID2);
	TArray<FVector> GetLocationsFromIDs(int64 ID1, int64 ID2);
};
