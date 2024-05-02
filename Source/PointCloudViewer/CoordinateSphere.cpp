// Fill out your copyright notice in the Description page of Project Settings.

#include "CoordinateSphere.h"

// Sets default values
ACoordinateSphere::ACoordinateSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMeshComponent"));
	RootComponent = SphereMeshComponent;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ACoordinateSphere::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* ParentActor = GetOwner())
	{
		InitialRelativePosition = GetActorLocation() - ParentActor->GetActorLocation();
	}
}

// Called every frame
void ACoordinateSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (AActor* ParentActor = GetOwner())
	{
		FRotator ParentRotation = ParentActor->GetActorRotation();
		
		FVector NewPosition = ParentRotation.RotateVector(InitialRelativePosition);
		SetActorLocation(ParentActor->GetActorLocation() + NewPosition);
	}
}

