// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCloudComponent.h"

TMap<FString,int32>  UPointCloudComponent::setClassificationIndices(TMap<FString, FLinearColor> StringMap)
{
	TMap<FString,int32> IndicesMap;

	for (const auto& item : StringMap)
	{
		if (!IndicesMap.Contains(item.Key))
		{
			IndicesMap.Add(item.Key,FCString::Atoi(*item.Key));
		}
	}

	return IndicesMap;
}

TMap<int32,FLinearColor> UPointCloudComponent::setClassificationItem(TMap<int32,FLinearColor>ColorsMap,TMap<FString,int32> IndexesMap, FString OldKey, FString NewKey, FLinearColor NewColor)
{
	if(NewKey == "")
	{
		NewKey = OldKey;
	}
	
	if (IndexesMap.Contains(OldKey))
	{
		int32 index = IndexesMap[OldKey];
		ColorsMap[index] = NewColor;
		IndexesMap.Remove(OldKey);
		IndexesMap.Add(NewKey, index);
	}

	return ColorsMap;
}

