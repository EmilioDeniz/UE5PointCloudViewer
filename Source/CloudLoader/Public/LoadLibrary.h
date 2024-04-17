// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "LidarPointCloud.h"
#include "HAL/PlatformFilemanager.h"
#include "LoadLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CLOUDLOADER_API ULoadLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Conversion")
	static TMap<FString,FLinearColor> convertToStringMap(TMap<int32,FLinearColor> intMap);

	UFUNCTION(BlueprintCallable, Category="FileView")
	static int64 getFileSize(FString FilePath);

	UFUNCTION(BlueprintCallable, Category="CloudLoad")
	static FVector GetPointCloudDimensions(ULidarPointCloud* PointCloud);

	UFUNCTION(BlueprintCallable, Category="CloudLoad")
	static FTransform GetPointCloudTransform(ULidarPointCloud* PointCloud, FVector Dimensions);
};

