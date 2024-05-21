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

USTRUCT(BlueprintType)
struct FClassifiedPoint
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Point Cloud")
	FLidarPointCloudPoint Point;
	
	UPROPERTY(BlueprintReadWrite, Category = "Point Cloud")
	FVector Location;

	UPROPERTY(BlueprintReadWrite, Category = "Point Cloud")
	int32 ClassificationID;
};

USTRUCT(BlueprintType)
struct FPowerLine
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FClassifiedPoint> Points;
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
	TArray<FClassifiedPoint> FilterPointsByID(int32 ClassID);
	
	UFUNCTION(BlueprintCallable,Category="PointCloudComponentUtils")
	TArray<FVector> ScanConflictingTrees();
private:
	FVector ScanTrees(const FClassifiedPoint& Point);
	TArray<FLidarPointCloudPoint> GetNearbyPoints(FVector Center, float SearchRadius);
};