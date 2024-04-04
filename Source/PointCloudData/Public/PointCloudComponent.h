// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HLSLTypeAliases.h"
#include "HLSLTypeAliases.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties", Meta = (ExposeOnSpawn = true))
	TMap<FString,FLinearColor> LabeledClassificationColors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties", Meta = (ExposeOnSpawn = true))
	TMap<FString,int32> LabeledClassificationIndices;
	
	UFUNCTION(BlueprintCallable,Category="PoitCloudComponentUtils")
	TMap<int32,FLinearColor> setClassificationItem(TMap<int32,FLinearColor> ColorsMap,TMap<FString,int32> IndexesMap,FString OldKey,FString NewKey,FLinearColor NewColor);

	UFUNCTION(BlueprintCallable,Category="PoitCloudComponentUtils")
	TMap<FString,int32>  setClassificationIndices(TMap<FString,FLinearColor>StringMap);
	
};
