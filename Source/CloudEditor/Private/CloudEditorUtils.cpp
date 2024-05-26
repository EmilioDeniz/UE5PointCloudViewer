// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudEditorUtils.h"

#include "EngineUtils.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"

FTransform UCloudEditorUtils::GetPawnTransform(APawn* Pawn, FVector Dimensions)
{
	FTransform Transform;

	if (Pawn)
	{
		float MaxDimension = FMath::Max3(Dimensions.X, Dimensions.Y, Dimensions.Z);
		
		FVector NewLocation = FVector(Dimensions.X / 4, Dimensions.Y / 4, -0.8 * MaxDimension / 10);
		
		Transform.SetLocation(NewLocation);
	}

	return Transform;
}

FVector UCloudEditorUtils::CalculateCorrectHitPoint(const FTransform& ActorTransform, const FVector& HitPoint)
{
	FQuat ActorRotation = ActorTransform.GetRotation();
	FVector Direction = (HitPoint - ActorTransform.GetLocation()).GetSafeNormal();
	FVector RotatedDirection = ActorRotation.Inverse().RotateVector(Direction);
	FVector CorrectHitPoint = ActorTransform.GetLocation() + RotatedDirection * (HitPoint - ActorTransform.GetLocation()).Size();
	return CorrectHitPoint;
}

AActor* UCloudEditorUtils::GetOwningActor(UUserWidget* UserWidget)
{
	if (!UserWidget)
	{
		return nullptr;
	}
	
	UWorld* World = UserWidget->GetWorld();
	if (!World)
	{
		return nullptr;
	}

	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		AActor* Actor = *ActorItr;

		TArray<UWidgetComponent*> WidgetComponents;
		Actor->GetComponents(WidgetComponents);

		for (UWidgetComponent* WidgetComponent : WidgetComponents)
		{
			if (WidgetComponent->GetUserWidgetObject() == UserWidget)
			{
				return Actor;
			}
		}
	}

	return nullptr;
}