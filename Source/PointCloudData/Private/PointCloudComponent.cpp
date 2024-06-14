// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCloudComponent.h"
#include "LidarPointCloud.h"
#include "SelectCubeActor.h"
#include "Components/BoxComponent.h"
#include "Math/UnitConversion.h"

void UPointCloudComponent::SetPointList()
{
	PointCloudPoints.Empty();
	GetPointCloud()->GetPoints(PointCloudPoints,0,-1);
}

void UPointCloudComponent::SetScanClasses()
{
	TArray<uint8> Cables = { 17, 18, 20, 13 };
	TArray<uint8> Trees = { 23, 3 };

	TArray<uint8> ImportedClasses = GetPointCloud()->GetClassificationsImported();

	for (uint8 Class : ImportedClasses)
	{
		if (Cables.Contains(Class))
		{
			CableClasses.Add(Class);
		}

		if (Trees.Contains(Class))
		{
			TreeClasses.Add(Class);
		}
	}
}

void UPointCloudComponent::SetSearchRadius(float Radius)
{
	SearchRadius = Radius;
}

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

void UPointCloudComponent::AddClassificationItemToList(FString Label, int32 ClassID)
{
	UClassificationItem* Item = NewObject<UClassificationItem>(this, UClassificationItem::StaticClass());
	FLinearColor RandomColor = FLinearColor(FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f), FMath::RandRange(0.f, 1.f));
	bool bContainsElement = false;
    
	for (UClassificationItem* ClassItem : ClassificationItemsList)
	{
		if (ClassItem->GetClassID() == ClassID)
		{
			bContainsElement = true;
			break;
		}
	}

	if (!bContainsElement)
	{
		if (Label.IsEmpty())
		{
			Label = FString::FromInt(ClassID);
		}
		Item->SetLabel(Label);
		Item->SetClassID(ClassID);
		Item->SetClassColor(RandomColor);
		
		ClassificationItemsList.Add(Item);
		ClassificationColors.Add(ClassID, RandomColor);
		ChangeClassificationColor(Item->GetClassID(), Item->GetClassColor());
	}
}

void UPointCloudComponent::RemoveItemFromList(int32 ClassID, bool CustomAddedOnly)
{
	if(CustomAddedOnly)
	{
		TArray<uint8> ImportedClasses = GetPointCloud()->GetClassificationsImported();
		if(ImportedClasses.Contains(uint8(ClassID)))
		{
			return;
		}
	}
	for (UClassificationItem* Item : ClassificationItemsList)
	{
		if (Item->GetClassID() == ClassID)
		{
			ClassificationItemsList.Remove(Item);
			ChangeClassificationColor(ClassID,FLinearColor::White);
			UpdateDeletedClassPoints(ClassID);
			break;
		}
	}
}

void UPointCloudComponent::ChangeClassificationColor(int32 Index, FLinearColor Color)
{
	if (ClassificationColors.Contains(Index))
	{
		ClassificationColors[Index] = Color;
	}
}

void UPointCloudComponent::ChangeClassificationItemColor(int32 Index, FLinearColor Color)
{
	for(UClassificationItem* Item: ClassificationItemsList)
	{
		if(Item->GetClassID() == Index)
		{
			Item->SetClassColor(Color);
			ChangeClassificationColor(Index,Color);
		}
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

FVector3f UPointCloudComponent::GetPointAtLocation(FVector Center)
{
	FVector Extent = {80,80,80};
	TArray<FLidarPointCloudPoint> Points = this->GetPointsInBoxAsCopies(Center,Extent,false,true);
	if(Points.Num()>0)
	{
		return Points[0].Location;
	}
	FVector3f Empty;
	return Empty;
}

void UPointCloudComponent::RotateAroundAxis(const FVector Axis, const FRotator Rotation)
{
	
	AActor* Parent = this -> GetOwner();
	if(Parent)
	{
		FTransform ParentTransform = Parent->GetActorTransform();
		
		FTransform AxisTransform = FTransform(ParentTransform.GetRotation(), Axis);
		FTransform DeltaPivotToOriginal = ParentTransform * AxisTransform.Inverse();
		FTransform DesiredTransform = FTransform(Rotation, FVector(0,0,0)) * DeltaPivotToOriginal * FTransform(Rotation, FVector(0,0,0)) * AxisTransform;

		Parent->SetActorRelativeTransform(DesiredTransform);
	}
}

TArray<FLidarPointCloudPoint*> UPointCloudComponent::FilterPointsByID(int32 ClassID)
{
    TArray<FLidarPointCloudPoint*> FilteredPoints;
    ULidarPointCloud* Cloud = GetPointCloud();
    if (Cloud)
    {
        for (FLidarPointCloudPoint* Point : PointCloudPoints)
        {
            if (Point->ClassificationID == ClassID)
            {
            	FilteredPoints.Add(Point);
            }
        }
    }
    return FilteredPoints;
}

TArray<FVector3f> UPointCloudComponent::ScanConflictingTrees(bool bReturnCoordinates)
{
	TArray<FVector3f> TreePoints;

	for (int32 Class : CableClasses)
	{
		TArray<FLidarPointCloudPoint*> Points = FilterPointsByID(Class);

		for (FLidarPointCloudPoint* Point : Points)
		{
			ScanTrees(Point);
		}
	}
	if(bReturnCoordinates)
{
		for(FLidarPointCloudPoint* Point: ScannedPoints)
		{
			TreePoints.Add(Point->Location);
		}
	}
	GetPointCloud()->RefreshRendering();
	ScannedPoints.Empty();
	return TreePoints;
}

void UPointCloudComponent::ScanTrees(FLidarPointCloudPoint* Point)
{
	if (!Point->Location.IsZero())
	{
		TArray<FLidarPointCloudPoint*> Points = GetNearbyPoints(Point, SearchRadius);
        
		for (FLidarPointCloudPoint* NearbyPoint : Points)
		{
			if (TreeClasses.Contains(uint8(NearbyPoint->ClassificationID)) && !ScannedPoints.Contains(NearbyPoint))
			{
				SaveOriginalColor(NearbyPoint, NearbyPoint->Color);

				NearbyPoint->Color = ProblemsColor;
				
				ScannedPoints.Add(NearbyPoint);
			}
		}
	}
}

TArray<FLidarPointCloudPoint*> UPointCloudComponent::GetNearbyPoints(FLidarPointCloudPoint* Center, float Radius)
{
   
	ULidarPointCloud* Cloud = GetPointCloud();
	TArray<FLidarPointCloudPoint*> NearbyPoints;
	FVector CenterPoint = FVector(Center->Location);
	const FSphere SearchSphere(CenterPoint, Radius);
    	
	Cloud->GetPointsInSphere(NearbyPoints, SearchSphere,false);
	return NearbyPoints;
}

void UPointCloudComponent::SaveOriginalColor(FLidarPointCloudPoint* Point, FColor Color)
{
	FPointColorTuple NewTuple;
	NewTuple.Point = Point;
	NewTuple.Color = Color;
	OriginalPointColors.Add(NewTuple);
}

void UPointCloudComponent::ResetPaintedPoints()
{
	if (!OriginalPointColors.IsEmpty())
	{
		for (FPointColorTuple Tuple : OriginalPointColors)
		{
			Tuple.Point->Color = Tuple.Color;
		}

		OriginalPointColors.Empty();
		GetPointCloud()->RefreshRendering();
	}
}

void UPointCloudComponent::ResetPaintedPoint(FLidarPointCloudPoint* Point)
{
	if (!OriginalPointColors.IsEmpty())
	{
		int32 IndexToRemove = INDEX_NONE;
		
		for (int32 Index = 0; Index < OriginalPointColors.Num(); ++Index)
		{
			if (OriginalPointColors[Index].Point == Point)
			{
				OriginalPointColors[Index].Point->Color = OriginalPointColors[Index].Color;
				IndexToRemove = Index;
				break;
			}
		}
		
		if (IndexToRemove != INDEX_NONE)
		{
			OriginalPointColors.RemoveAt(IndexToRemove);
		}
	}
}

void UPointCloudComponent::SelectPoints(FVector Center, FBox StartingBox)
{
	FVector SelectCenter = Center;
	FVector BoxExtent = StartingBox.GetExtent();
	
	FVector BoxMin = SelectCenter - BoxExtent;
	FVector BoxMax = SelectCenter + BoxExtent;

	FVector CorrectedBoxMin = FVector(FMath::Min(BoxMin.X, BoxMax.X), FMath::Min(BoxMin.Y, BoxMax.Y), FMath::Min(BoxMin.Z, BoxMax.Z));
	FVector CorrectedBoxMax = FVector(FMath::Max(BoxMin.X, BoxMax.X), FMath::Max(BoxMin.Y, BoxMax.Y), FMath::Max(BoxMin.Z, BoxMax.Z));
	
	StartingBox.Min = CorrectedBoxMin;
	StartingBox.Max = CorrectedBoxMax;
	
	TArray<FLidarPointCloudPoint*> PointsInBox;
	GetPointsInBox(PointsInBox,StartingBox,false);
	
	for (FLidarPointCloudPoint* Point : PointsInBox)
	{
		SaveOriginalColor(Point,Point->Color);
		Point->Color = SelectionColor;
		SelectedPoints.Add(Point);
	}
	GetPointCloud()->RefreshRendering();
	bPointsSelected = !SelectedPoints.IsEmpty();
}

void UPointCloudComponent::DeselectPoints(FVector Center, FBox StartingBox)
{
	if(bPointsSelected)
	{
		FVector SelectCenter = Center;
		FVector BoxExtent = StartingBox.GetExtent();
        	
		FVector BoxMin = SelectCenter - BoxExtent;
		FVector BoxMax = SelectCenter + BoxExtent;
        
		FVector CorrectedBoxMin = FVector(FMath::Min(BoxMin.X, BoxMax.X), FMath::Min(BoxMin.Y, BoxMax.Y), FMath::Min(BoxMin.Z, BoxMax.Z));
		FVector CorrectedBoxMax = FVector(FMath::Max(BoxMin.X, BoxMax.X), FMath::Max(BoxMin.Y, BoxMax.Y), FMath::Max(BoxMin.Z, BoxMax.Z));
        	
		StartingBox.Min = CorrectedBoxMin;
		StartingBox.Max = CorrectedBoxMax;
        	
		TArray<FLidarPointCloudPoint*> PointsInBox;
		GetPointsInBox(PointsInBox,StartingBox,false);
        	
		for (FLidarPointCloudPoint* Point : PointsInBox)
		{
			if(SelectedPoints.Contains(Point))
			{
				ResetPaintedPoint(Point);
				SelectedPoints.Remove(Point);
			}
		}
		GetPointCloud()->RefreshRendering();
		bPointsSelected = !SelectedPoints.IsEmpty();
	}
}

void UPointCloudComponent::DeselectAllPoints()
{
	if(bPointsSelected)
	{
		ResetPaintedPoints();
		SelectedPoints.Empty();
		bPointsSelected = !SelectedPoints.IsEmpty();
	}
}

void UPointCloudComponent::ChangeSelectedPointsClass(int32 ClassID)
{
	if (bPointsSelected)
	{
		for (FLidarPointCloudPoint* Point : SelectedPoints)
		{
			Point->ClassificationID = uint8(ClassID);
		}
	}
}

void UPointCloudComponent::UpdateDeletedClassPoints(int32 DeletedClassID)
{
	SetPointList();
	TArray<FLidarPointCloudPoint*> FilteredPoints = FilterPointsByID(DeletedClassID);

	for(FLidarPointCloudPoint* Point: FilteredPoints)
	{
		Point->ClassificationID = 1;
	}

	if(ClassificationColors.Contains(DeletedClassID))
	{
		ClassificationColors.Remove(DeletedClassID);
	}

	if(!ClassificationColors.Contains(1))
	{
		ClassificationColors.Add(1);
	}
}

UClassificationItem* UPointCloudComponent::GetClassificationItem(int32 ClassID)
{
	for(UClassificationItem* Item: ClassificationItemsList)
	{
		if(Item->GetClassID() == ClassID)
		{
			return Item;
		}
	}
	return nullptr;
}

UClassificationItem* UPointCloudComponent::GetClassificationItemByName(FString Label)
{
	for(UClassificationItem* Item: ClassificationItemsList)
	{
		if(Item->GetLabel() == Label)
		{
			return Item;
		}
	}
	return nullptr;
}

FBox UPointCloudComponent::GetStartingBox(ASelectCubeActor* Cube)
{
	return Cube->CalculateComponentsBoundingBoxInLocalSpace();
}

FVector UPointCloudComponent::GetCenter(ASelectCubeActor* Cube)
{
	FVector RelativePosition = Cube->GetActorLocation() - GetOwner()->GetActorLocation();
	FVector Center = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(RelativePosition);
	return Center;
}

void UPointCloudComponent::SetSelectionColor(FLinearColor Color)
{
	SelectionColor = Color.ToFColor(true);
}

FLinearColor UPointCloudComponent::GetSelectionColor()
{
	return FLinearColor(SelectionColor);
}

void UPointCloudComponent::SetProblemsColor(FLinearColor Color)
{
	ProblemsColor = Color.ToFColor(true);
}

FLinearColor UPointCloudComponent::GetProblemsColor()
{
	return FLinearColor(ProblemsColor);
}

TArray<UClassificationItem*> UPointCloudComponent::GetClassificationItemsList()
{
	return ClassificationItemsList;
}

bool UPointCloudComponent::GetBPointsSelected()
{
	return bPointsSelected;
}
