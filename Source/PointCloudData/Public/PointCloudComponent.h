// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HLSLTypeAliases.h"
#include "HLSLTypeAliases.h"
#include "ClassificationItem.h"
#include "LidarPointCloudComponent.h"
#include "PointCloudComponent.generated.h"


/**
 * 
 */
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
};
