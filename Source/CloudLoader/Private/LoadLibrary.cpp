// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadLibrary.h"


TMap<FString, FLinearColor> ULoadLibrary::convertToStringMap(TMap<int32, FLinearColor> intMap)
{
	TMap<FString, FLinearColor> stringMap;

	for (const auto& item : intMap)
	{
		FString stringKey = FString::Printf(TEXT("%d"), item.Key);
		stringMap.Add(stringKey, item.Value);
	}

	return stringMap;
}

int64 ULoadLibrary::getFileSize(FString FilePath)
{
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (platformFile.FileExists(*FilePath))
	{
		int64 fileSize = platformFile.FileSize(*FilePath);
		return fileSize / (1024 * 1024);
	}

	return 0.0;
}

FVector ULoadLibrary::GetPointCloudDimensions(ULidarPointCloud* PointCloud)
{
	if(PointCloud)
	{
		FBoxSphereBounds Bounds = PointCloud->GetBounds();
		FVector Min = Bounds.Origin - Bounds.BoxExtent;
		FVector Max = Bounds.Origin + Bounds.BoxExtent;

		return Max - Min;
	}

	return FVector::ZeroVector;
}
FTransform ULoadLibrary::GetPointCloudTransform(ULidarPointCloud* PointCloud, FVector Dimensions)
{
	FTransform Transform;

	if(PointCloud)
	{
		FBoxSphereBounds Bounds = PointCloud->GetBounds();
		float MaxDimension = FMath::Max(Dimensions.X, FMath::Max(Dimensions.Y, Dimensions.Z));

		FVector NewLocation = Bounds.Origin + FVector(0, 0, -MaxDimension/2);
		FRotator NewRotation = FRotator(45, 0, 0); 

		Transform.SetLocation(NewLocation);
		Transform.SetRotation(NewRotation.Quaternion());
	}

	return Transform;
}




	