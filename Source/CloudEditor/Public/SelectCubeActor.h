// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectCubeActor.generated.h"

UCLASS()
class CLOUDEDITOR_API ASelectCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectCubeActor();

	UPROPERTY(BlueprintReadWrite, Category = "Selection",meta=(ExposeOnSpawn))
	TArray<FVector> Vertices;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category="Selection")
	void CalculateAndSetScale(UStaticMeshComponent* Cube);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	FVector InitialRelativePosition;
	FRotator InitialRelativeRotation;
};
