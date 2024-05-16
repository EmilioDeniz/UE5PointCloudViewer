// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudEditorUtils.h"

#include "EngineUtils.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"

void UCloudEditorUtils::GenerateClassColorsPanelWithString(TMap<FString, FLinearColor> StringColorMap, UScrollBox* ScrollBox, UObject* WorldContextObject)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/ClassificationColorWidgetBlueprint.ClassificationColorWidgetBlueprint'"));
	if (WidgetClass.Succeeded())
	{
		for (const auto& Elem : StringColorMap)
		{
			AddWidgetToScrollBox(Elem.Key, Elem.Value, ScrollBox, WorldContextObject, WidgetClass.Class);
		}
	}
}

void UCloudEditorUtils::GenerateClassColorsPanelWithInt(UScrollBox* ScrollBox, UObject* WorldContextObject, TMap<int32, FLinearColor> IntColorMap)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/ClassificationColorWidgetBlueprint.ClassificationColorWidgetBlueprint'"));
	if (WidgetClass.Succeeded())
	{
		for (const auto& Elem : IntColorMap)
		{
			AddWidgetToScrollBox(FString::FromInt(Elem.Key), Elem.Value, ScrollBox, WorldContextObject, WidgetClass.Class);
		}
	}
}

void UCloudEditorUtils::AddWidgetToScrollBox(FString Key, FLinearColor Color, UScrollBox* ScrollBox, UObject* WorldContextObject, UClass* WidgetClass)
{
	UWorld* World = WorldContextObject->GetWorld();
	UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(World, WidgetClass);
            
	if (WidgetInstance)
	{
		// Cambia el color del botón
		if (UButton* Button = Cast<UButton>(WidgetInstance->GetWidgetFromName(TEXT("Color_Button"))))
		{
			Button->SetColorAndOpacity(Color);
		}
		
		if (UEditableText* EditableText = Cast<UEditableText>(WidgetInstance->GetWidgetFromName(TEXT("Color_ID_Text"))))
		{
			EditableText->SetText(FText::FromString(Key));
		}
		WidgetInstance->SetVisibility(ESlateVisibility::Visible);		
		ScrollBox->AddChild(WidgetInstance);
		WidgetInstance->AddToViewport();
	} 
}

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