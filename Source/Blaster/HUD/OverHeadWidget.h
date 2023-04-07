// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DisplayText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerName;

	void SetDisplayText(FString TextToDisplay);
	void SetPlayerName(FString NameToDisPlay);
	UFUNCTION(BlueprintCallable)
	void ShowPLayerNetRole(APawn* InPawn);
	UFUNCTION(BlueprintCallable)
	void ShowPLayerName(APawn* InPawn);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)override;
};
