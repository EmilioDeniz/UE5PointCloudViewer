// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCloudComponent.h"
#include "LidarPointCloud.h"

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

FVector3f UPointCloudComponent::GetPointAtLocation(FVector Center)
{
	FVector Extent = {80,80,80};
	TArray<FLidarPointCloudPoint> Points = this->GetPointsInBoxAsCopies(Center,Extent,false,true);
	if(Points.Num()>0)
	{
		return Points[0].Location;
	}else
	{
		 FVector3f Empty;
		 return Empty;
	}
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

TArray<FClassifiedPoint> UPointCloudComponent::FilterPointsByID(int32 ClassID)
{
    TArray<FClassifiedPoint> FilteredPoints;
    ULidarPointCloud* Cloud = GetPointCloud();
    if (Cloud)
    {
        FTransform PointCloudTransform = GetComponentTransform();

        TArray<FLidarPointCloudPoint> PointsArray;
        Cloud->GetPointsAsCopies(PointsArray, true, 0, -1);
        
        for (FLidarPointCloudPoint Point : PointsArray)
        {
            if (Point.ClassificationID == ClassID)
            {
                FClassifiedPoint FilteredPoint;
                FilteredPoint.Location = FVector(Point.Location);
                FilteredPoint.ClassificationID = Point.ClassificationID;
                FilteredPoints.Add(FilteredPoint);
            }
        }
    }

    return FilteredPoints;
}

TArray<FVector> UPointCloudComponent::ScanConflictingTrees()
{
    TArray<FVector> TreePoints;
    
    const TArray<int32> CableClasses = {17, 18, 20, 13};
	
    for(int32 Class : CableClasses)
    {
        TArray<FClassifiedPoint> Points = FilterPointsByID(Class);

        for(FClassifiedPoint Point : Points)
        {
            FVector TreePoint = ScanTrees(Point);
            if (!TreePoint.IsZero())
            {
                TreePoints.Add(TreePoint);
            }
        }
    }
    return TreePoints;
}

FVector UPointCloudComponent::ScanTrees(const FClassifiedPoint& Point)
{
    if (!Point.Location.IsZero())
    {
        TArray<FLidarPointCloudPoint> Points = GetNearbyPoints(Point.Location, 100.0f);
        for (FLidarPointCloudPoint NearbyPoint : Points)
        {
            if (NearbyPoint.ClassificationID == 23|| NearbyPoint.ClassificationID == 3)
            {
                return FVector(NearbyPoint.Location);
            }
        }
    }
    return FVector::ZeroVector;
}

TArray<FLidarPointCloudPoint> UPointCloudComponent::GetNearbyPoints(FVector Center, float SearchRadius)
{
    if (this && GetPointCloud())
    {
        ULidarPointCloud* Cloud = GetPointCloud();
        TArray<FLidarPointCloudPoint> NearbyPoints = Cloud->GetPointsInSphereAsCopies(Center, SearchRadius, false, true);
        return NearbyPoints;
    }
    return {};
}
