// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HLSLTypeAliases.h"
#include "HLSLTypeAliases.h"
#include "ClassificationItem.h"
#include "Templates/Function.h"
#include "LidarPointCloud.h"
#include "Math/UnrealMathUtility.h"
#include "SelectCubeActor.h"
#include "LidarPointCloudComponent.h"
#include "PointCloudComponent.generated.h"


/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FPointCloudFilterCallback, const TArray<FVector>&, FilteredPoints);

USTRUCT()
struct FPointColorTuple
{
	GENERATED_BODY()
	
	FLidarPointCloudPoint * Point;
	FColor Color;
};

UCLASS()
class POINTCLOUDDATA_API UPointCloudComponent : public ULidarPointCloudComponent
{
	GENERATED_BODY()
public:
	
	void ResetPaintedPoint(FLidarPointCloudPoint* Point);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties")
	TArray<UClassificationItem*> ClassificationItemsList;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom Properties")
	bool bPointsSelected = false;
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void SetClassificationItemsList(TMap<int32,FLinearColor>ColorMap);

	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void AddClassificationItemToList(FString Label, int32 ClassID);

	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void RemoveItemFromList(int32 ClassID, bool CustomAddedOnly);

	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void ChangeClassificationColor(int32 Index, FLinearColor Color);

	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void ChangeItemName(int32 Index, FString NewName);
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	FVector3f GetPointAtLocation(FVector Center);
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void RotateAroundAxis(const FVector Axis, const FRotator Rotation);
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void SetPointList();
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void SetScanClasses();
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void SetSearchRadius(float Radius);

	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	TArray<FVector3f> ScanConflictingTrees(bool bReturnCoordinates);

	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void ResetPaintedPoints();
	
	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void SelectPoints(FVector Center, FBox StartingBox);
	
	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void DeselectPoints(FVector Center, FBox StartingBox);
	
	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void DeselectAllPoints();

	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void ChangeSelectedPointsClass(int32 ClassID);
	
	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	UClassificationItem* GetClassificationItem(int32 ClassID);
	
	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	UClassificationItem* GetClassificationItemByName(FString Label);

	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	FBox GetStartingBox(ASelectCubeActor* Cube);
	
	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	FVector GetCenter(ASelectCubeActor* Cube);

	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void SetSelectionColor(FLinearColor Color);
	
	UFUNCTION(BlueprintPure, Category="PointCloudComponentUtils")
	FLinearColor GetSelectionColor();

	UFUNCTION(BlueprintCallable, Category="PointCloudComponentUtils")
	void SetProblemsColor(FLinearColor Color);
	
	UFUNCTION(BlueprintPure, Category="PointCloudComponentUtils")
	FLinearColor GetProblemsColor();

private:
	TArray<FLidarPointCloudPoint*> FilterPointsByID(int32 ClassID);
	void ScanTrees(FLidarPointCloudPoint* Point);
	TArray<FLidarPointCloudPoint*> GetNearbyPoints(FLidarPointCloudPoint* Center, float SearchRadius);
	void SaveOriginalColor(FLidarPointCloudPoint*Point, FColor Color);
	void UpdateDeletedClassPoints(int32 DeletedClass);
	
	TArray<FLidarPointCloudPoint*> PointCloudPoints;
	TArray<uint8> CableClasses;
	TArray<uint8> TreeClasses;
	TArray<FPointColorTuple> OriginalPointColors;
	TArray<FLidarPointCloudPoint*> ScannedPoints;
	TArray<FLidarPointCloudPoint*> SelectedPoints;
	FColor SelectionColor = FColor::Orange;
	FColor ProblemsColor = FColor::Red;
	float SearchRadius;
};