// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DistanceLine.generated.h"

UCLASS()
class POINTCLOUDDATA_API ADistanceLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADistanceLine();

	UPROPERTY(BlueprintReadWrite,Category = "Distance Line", Meta = (ExposeOnSpawn = true))
	FVector Distance;

	UPROPERTY(BlueprintReadWrite,Category = "Distance Line", Meta = (ExposeOnSpawn = true))
	TArray<AActor*> Actors;

	UPROPERTY(BlueprintReadWrite,Category = "Distance Line", Meta = (ExposeOnSpawn = true))
	TArray<FVector> CorrectLocations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LineMeshComponent;
	FVector InitialRelativePosition;

	UFUNCTION(BlueprintCallable,Category= "Distance Line")
	void SetLineLocationAndRotation(UStaticMeshComponent* Line);

	UFUNCTION(BlueprintCallable, Category= "Distance Line")
	void SetLineLength(UStaticMeshComponent* Line);
};
