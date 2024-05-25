// Fill out your copyright notice in the Description page of Project Settings.


#include "PointCloudComponent.h"
#include "LidarPointCloud.h"
#include "SelectCubeActor.h"
#include "Components/BoxComponent.h"

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

				NearbyPoint->Color = FColor::Red;
				
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

void UPointCloudComponent::SelectPoints(ASelectCubeActor* Cube, UStaticMeshComponent* CubeComponent)
{
	if (!Cube || !CubeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor or CubeComponent is null."));
		return;
	}
	
	FTransform ActorTransform = Cube->GetActorTransform();
	
	FVector ComponentScale = CubeComponent->GetComponentScale();
	FRotator ComponentRotation = CubeComponent->GetComponentRotation();
	FVector ComponentLocation = CubeComponent->GetComponentLocation();

	FVector BoxExtent = CubeComponent->GetStaticMesh()->GetBounds().BoxExtent * ComponentScale;

	FVector BoxCenter = ComponentLocation;

	DrawDebugBox(GetWorld(), BoxCenter, BoxExtent, ComponentRotation.Quaternion(), FColor::Green, true, 5.0f, 0, 10.0f);
}


