// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ElimAnnouncement.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UElimAnnouncement : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void SetAnnouncementText(const FString& AttackerName, const FString& VictimName);
	void SetAnnouncementText(int32 EnemiesLeft);

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* AnnouncementBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AnnouncementText;
};
