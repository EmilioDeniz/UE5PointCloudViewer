// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCubeActor.h"

// Sets default values
ASelectCubeActor::ASelectCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASelectCubeActor::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* ParentActor = GetOwner())
	{
		InitialRelativePosition = GetActorLocation() - ParentActor->GetActorLocation();
		InitialRelativePosition = GetActorLocation() - ParentActor->GetActorLocation();
	}
	
}

// Called every frame
void ASelectCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AActor* ParentActor = GetOwner())
	{
		FVector NewPosition = ParentActor->GetActorLocation() + ParentActor->GetActorRotation().RotateVector(InitialRelativePosition);
		SetActorLocation(NewPosition);

		FRotator NewRotation = ParentActor->GetActorRotation() + InitialRelativeRotation;
		SetActorRotation(NewRotation);
	}
}

void ASelectCubeActor::CalculateAndSetScale(UStaticMeshComponent* Cube)
{
	if(Vertices.Num() == 2)
	{
		FVector Opposite2 = FVector(Vertices[0].X, Vertices[1].Y, Vertices[0].Z);

		float Height = FVector::Dist(Vertices[1], Opposite2) / 100.0f;
		float Width = FVector::Dist(Vertices[0], Opposite2) / 100.0f;

		FVector Distance = Vertices[0] - Vertices[1];

		bool bDifferenceInY = FMath::Abs(Distance.Y) > FMath::Abs(Distance.Z);
		bool bDifferenceInZ = !bDifferenceInY;

		if (bDifferenceInY)
		{
			Cube->SetWorldScale3D(FVector(Height,Width,Width));
		}
		else if (bDifferenceInZ)
		{
			Cube->SetWorldScale3D(FVector(Width,Width,Height));
		}
	}
}