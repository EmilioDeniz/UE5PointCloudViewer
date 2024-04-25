// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudViewPawn.h"
#include "Camera/CameraComponent.h"
#include "InputCoreTypes.h"

// Sets default values
ACloudViewPawn::ACloudViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	
	Zoom = 0.1f;
	MinZoom = 0.1f;
	MaxZoom = 3.0f;
	InterpSpeed = 5.0f;
}

// Called when the game starts or when spawned
void ACloudViewPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACloudViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACloudViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACloudViewPawn::HandleZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ACloudViewPawn::HandleZoomOut);
}

void ACloudViewPawn::SetPawnTransform(FVector Dimensions)
{
	if(this)
	{
		
		FVector NewLocation = FVector(0.359*Dimensions.X, -0.308*Dimensions.Y, 0.494*Dimensions.Z);
		FRotator NewRotation = FRotator(327.365383, 85.445007, 0); 

		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
}


void ACloudViewPawn::HandleZoomIn()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	float TargetFOV = FMath::Max(CameraComponent->FieldOfView - Zoom, MinZoom);
	
	float NewFOV = FMath::Lerp(CameraComponent->FieldOfView, TargetFOV, DeltaTime * InterpSpeed);
	CameraComponent->SetFieldOfView(FMath::Clamp(NewFOV, MinZoom, MaxZoom));
}

void ACloudViewPawn::HandleZoomOut()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	float TargetFOV = FMath::Min(CameraComponent->FieldOfView + Zoom, MaxZoom);
	
	float NewFOV = FMath::Lerp(CameraComponent->FieldOfView, TargetFOV, DeltaTime * InterpSpeed);
	CameraComponent->SetFieldOfView(FMath::Clamp(NewFOV, MinZoom, MaxZoom));
}



