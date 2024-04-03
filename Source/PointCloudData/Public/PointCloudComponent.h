// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void setClassificationItem(FString OldKey,FString NewKey,FLinearColor NewColor);

	UFUNCTION(BlueprintCallable,Category="PoitCloudComponentUtils")
	void setClassificationIndices(TMap<FString,FLinearColor>StringMap);
};
