// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudEditorUtils.h"

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

		// Accede a la variable Default_Text y la establece con el nombre de la clase
		if (UEditableText* EditableText = Cast<UEditableText>(WidgetInstance->GetWidgetFromName(TEXT("Color_ID_Text"))))
		{
			EditableText->SetText(FText::FromString(Key));
		}
		WidgetInstance->SetVisibility(ESlateVisibility::Visible);		
		ScrollBox->AddChild(WidgetInstance);
		WidgetInstance->AddToViewport();
	} 
}







