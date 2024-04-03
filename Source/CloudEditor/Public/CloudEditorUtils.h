// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "CloudEditorUtils.generated.h"



/**
 * 
 */

UCLASS()
class CLOUDEDITOR_API UCloudEditorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static void AddWidgetToScrollBox(FString Key, FLinearColor Color, UScrollBox* ScrollBox, UObject* WorldContextObject, UClass* WidgetClass);
public:
public:
	UFUNCTION(BlueprintCallable, Category = "Cloud Editor Utils")
	static void GenerateClassColorsPanelWithString(TMap<FString, FLinearColor> StringColorMap, UScrollBox* ScrollBox, UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Cloud Editor Utils")
	static void GenerateClassColorsPanelWithInt(UScrollBox* ScrollBox, UObject* WorldContextObject, TMap<int32, FLinearColor> IntColorMap);
};