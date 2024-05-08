// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceLine.h"


// Sets default values
ADistanceLine::ADistanceLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LineMesh"));
	RootComponent = LineMeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	if (CylinderMeshAsset.Succeeded())
	{
		LineMeshComponent->SetStaticMesh(CylinderMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ADistanceLine::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* ParentActor = GetOwner())
	{
		InitialRelativePosition = GetActorLocation() - ParentActor->GetActorLocation();
	}
}

// Called every frame
void ADistanceLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (Actors.Num() > 0)
	{
		AActor* Actor0 = Actors[0];
		FRotator Actor0Rotation = Actor0->GetActorRotation();
		
		SetActorRotation(Actor0Rotation);
	}*/
}

void ADistanceLine::SetLineLocationAndRotation(UStaticMeshComponent* Line)
{
	if(AActor* Parent = GetOwner())
	{
		if (Actors.Num() == 2) 
		{
			FVector Actor1Location = CorrectLocations[0];
			FVector Actor2Location = CorrectLocations[1];
        		
			FVector MiddlePoint = (Actor1Location + Actor2Location) / 2;
        		
			const float VerticalOffset = 50.0f; 
			MiddlePoint.Z += VerticalOffset;
        		
			this->SetActorLocation(MiddlePoint);
        		
			FVector Direction = Actor2Location - Actor1Location;
			FRotator Rotation = Direction.Rotation();
			Rotation.Pitch += 90.0f;
			
			FRotator WorldRotation = Parent->GetTransform().TransformRotation(Rotation.Quaternion()).Rotator();

			Line->SetWorldRotation(WorldRotation);
		}
	}
}

void ADistanceLine::SetLineLength(UStaticMeshComponent* Line)
{
	if (Actors.Num() == 2) 
	{
		float ScaleZ = Distance.Size()/100.f; 
		
		Line->SetWorldScale3D(FVector(0.5, 0.5, ScaleZ)); 
	}
}


