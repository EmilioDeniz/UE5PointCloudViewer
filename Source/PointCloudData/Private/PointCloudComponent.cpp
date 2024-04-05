// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCloudComponent.h"

void UPointCloudComponent::SetClassificationItemsList(TMap<int32, FLinearColor> ColorMap)
{
	TArray<UClassificationItem*> ItemsArray;
	
	for (const TPair<int32, FLinearColor>& Entry : ColorMap)
	{
	
		FString Name = FString::FromInt(Entry.Key);
		
		int32 Index = Entry.Key;
		FLinearColor OriginalColor = Entry.Value;
		
		FLinearColor RandomColor = FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f));
		
		UClassificationItem* Item = NewObject<UClassificationItem>(this, UClassificationItem::StaticClass());
		Item->SetLabel(Name);
		Item->SetClassID(Index);
		Item->SetClassColor(RandomColor);
		
		ItemsArray.Add(Item);
	}
	
	ClassificationItemsList = ItemsArray;

	for (UClassificationItem* Item : ClassificationItemsList)
	{
		ChangeClassificationColor(Item->GetClassID(), Item->GetClassColor());
	}
	
}

void UPointCloudComponent::ChangeClassificationColor(int32 Index, FLinearColor Color)
{
	if (ClassificationColors.Contains(Index))
	{
		ClassificationColors[Index] = Color;
	}
}

void UPointCloudComponent::ChangeItemName(int32 Index, FString NewName)
{
	for (UClassificationItem* Item : ClassificationItemsList)
	{
		if (Item->GetClassID() == Index)
		{
			Item->SetLabel(NewName);
			break; 
		}
	}
}



