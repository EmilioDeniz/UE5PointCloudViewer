// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LidarPointCloudActor.h"
#include "PointCloudActor.generated.h"

/**
 * 
 */
UCLASS()
class POINTCLOUDDATA_API APointCloudActor : public ALidarPointCloudActor
{
	GENERATED_BODY()
	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties")
			FString DirectoryPath;
};
