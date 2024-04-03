// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCloudComponent.h"

void UPointCloudComponent::setClassificationIndices(TMap<FString, FLinearColor> StringMap)
{
	TMap<int32,FLinearColor> ColorsMap = ClassificationColors;
	TMap<FString,int32> IndicesMap;

	for (const auto& item : StringMap)
	{
		for (const auto& colorItem : ColorsMap)
		{
			if (item.Value == colorItem.Value)
			{
				IndicesMap.Add(item.Key, colorItem.Key);
				break;
			}
		}
	}

	LabeledClassificationIndices = IndicesMap;
}

void UPointCloudComponent::setClassificationItem(FString OldKey, FString NewKey, FLinearColor NewColor)
{
	TMap<int32,FLinearColor> ColorsMap = ClassificationColors;
	TMap<FString,FLinearColor> LabeledMap = LabeledClassificationColors;
	TMap<FString,int32> IndexesMap = LabeledClassificationIndices;

	if (IndexesMap.Contains(OldKey))
	{
		int32 index = IndexesMap[OldKey];
		ColorsMap[index] = NewColor;
		IndexesMap.Remove(OldKey);
		IndexesMap.Add(NewKey, index);
		LabeledMap.Remove(OldKey);
		LabeledMap.Add(NewKey, NewColor);
	}
}
