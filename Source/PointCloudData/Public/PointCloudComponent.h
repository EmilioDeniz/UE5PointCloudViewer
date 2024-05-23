// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HLSLTypeAliases.h"
#include "HLSLTypeAliases.h"
#include "ClassificationItem.h"
#include "Templates/Function.h"
#include "LidarPointCloud.h"
#include "Math/UnrealMathUtility.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties")
	TArray<UClassificationItem*> ClassificationItemsList;
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	void SetClassificationItemsList(TMap<int32,FLinearColor>ColorMap);

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

private:
	TArray<FLidarPointCloudPoint*> FilterPointsByID(int32 ClassID);
	void ScanTrees(FLidarPointCloudPoint* Point);
	TArray<FLidarPointCloudPoint*> GetNearbyPoints(FLidarPointCloudPoint* Center, float SearchRadius);
	void SaveOriginalColor(FLidarPointCloudPoint*Point, FColor Color);

	TArray<FLidarPointCloudPoint*> PointCloudPoints;
	TArray<uint8> CableClasses;
	TArray<uint8> TreeClasses;
	TArray<FPointColorTuple> OriginalPointColors;
	TArray<FLidarPointCloudPoint*> ScannedPoints;
	float SearchRadius;
};