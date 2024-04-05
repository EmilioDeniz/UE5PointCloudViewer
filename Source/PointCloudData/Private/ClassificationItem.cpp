#include "ClassificationItem.h"

UClassificationItem::UClassificationItem()
{
	Label = "";
	ClassID = 0;
	ClassColor = FLinearColor::White;
}

UClassificationItem::UClassificationItem(FString Name, int32 Index, FLinearColor Color)
: Label(Name), ClassID(Index), ClassColor(Color)
{
}

UClassificationItem::~UClassificationItem()
{
}

void UClassificationItem::SetLabel(FString Name)
{
	Label = Name;
}

void UClassificationItem::SetClassID(int32 Index)
{
	ClassID = Index;
}

void UClassificationItem::SetClassColor(FLinearColor Color)
{
	ClassColor = Color;
}

FString UClassificationItem::GetLabel()
{
	return Label;
}

int32 UClassificationItem::GetClassID()
{
	return ClassID;
}

FLinearColor UClassificationItem::GetClassColor()
{
	return ClassColor;
}

