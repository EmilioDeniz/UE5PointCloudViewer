#pragma once

#include "CoreMinimal.h"
#include "ClassificationItem.generated.h"

UCLASS()
class POINTCLOUDDATA_API UClassificationItem : public UObject
{
	GENERATED_BODY()

public:
	UClassificationItem();
	UClassificationItem(FString name, int32 index, FLinearColor color);
	virtual ~UClassificationItem();

	UFUNCTION(BlueprintCallable, Category = "ClassificationItem")
	void SetLabel(FString Name);

	UFUNCTION(BlueprintCallable, Category = "ClassificationItem")
	void SetClassID(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "ClassificationItem")
	void SetClassColor(FLinearColor Color);

	UFUNCTION(BlueprintCallable, Category = "ClassificationItem")
	FString GetLabel();

	UFUNCTION(BlueprintCallable, Category = "ClassificationItem")
	int32 GetClassID();

	UFUNCTION(BlueprintCallable, Category = "ClassificationItem")
	FLinearColor GetClassColor();

private:
	FString Label;
	int32 ClassID;
	FLinearColor ClassColor;
};



