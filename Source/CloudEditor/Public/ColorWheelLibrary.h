// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ColorWheelLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CLOUDEDITOR_API UColorWheelLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	private:
		static float VectorToRadialValue(const FVector2D& Vector);
		static float CalculateSin(float Angle);
	public:
		UFUNCTION(BlueprintCallable, Category = "Math|Conversion")
		static FVector PickColorFromWheel(const FVector2D& Coordinates);
};
