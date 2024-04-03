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
