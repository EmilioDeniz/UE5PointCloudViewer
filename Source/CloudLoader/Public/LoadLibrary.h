// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
    
};
