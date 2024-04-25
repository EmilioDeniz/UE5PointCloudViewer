// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CloudViewPawn.generated.h"

UCLASS()
class CLOUDEDITOR_API ACloudViewPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACloudViewPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void HandleZoomIn();
	void HandleZoomOut();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "View Pawn")
	void SetPawnTransform(FVector Dimensions);

	UPROPERTY(BlueprintReadWrite)
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float Zoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxZoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "View Pawn Camera")
	bool bRightClicked;
};
