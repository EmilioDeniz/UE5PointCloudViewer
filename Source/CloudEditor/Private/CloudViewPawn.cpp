// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudViewPawn.h"

// Sets default values
ACloudViewPawn::ACloudViewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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


