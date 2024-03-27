// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorWheelLibrary.h"

float  UColorWheelLibrary::VectorToRadialValue(const FVector2D& Vector)
{
	return FMath::Atan2(Vector.Y, Vector.X);
}
float  UColorWheelLibrary::CalculateSin(float Angle)
{
	return FMath::Sin(Angle);
}

FVector UColorWheelLibrary::PickColorFromWheel(const FVector2D& Coordinates)
{
	const FVector2d NormalizedVector = Coordinates - FVector2d(0.5f,0.5f);

	const float Angle = VectorToRadialValue(NormalizedVector);

	const float SinValueX = CalculateSin(Angle);
	const float SinValueY = CalculateSin(Angle+ 2.0944f);
	const float SinValueZ = CalculateSin(Angle+ 4.18879f);

	const float ValueX = FMath::Clamp(SinValueX, 0.0f, 1.0f);
	const float ValueY = FMath::Clamp(SinValueY, 0.0f, 1.0f);
	const float ValueZ = FMath::Clamp(SinValueZ, 0.0f, 1.0f);

	float Length = NormalizedVector.Size();
	float ClampValue = FMath::Clamp(Length*2.0f,0.0f,1.0f);
	float Alpha = FMath::Pow(ClampValue-1.0f,5);


	FVector ColorVector(ValueX, ValueY, ValueZ);
	FVector MultipliedColorVector = ColorVector * ColorVector;
	FVector UnitaryVector = FVector(1.0f,1.0f,1.0f);
	
	FVector ResultVector = FMath::Lerp(MultipliedColorVector,UnitaryVector,Alpha);
	return ResultVector;
}