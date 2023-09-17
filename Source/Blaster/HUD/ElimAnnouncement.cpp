// Fill out your copyright notice in the Description page of Project Settings.


#include "ElimAnnouncement.h"
#include "Components/TextBlock.h"

void UElimAnnouncement::SetAnnouncementText(const FString& AttackerName, const FString& VictimName)
{
	FString ElimAnnouncementText = FString::Printf(TEXT("%s elimed %s!"), *AttackerName, *VictimName);
	if (AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}

void UElimAnnouncement::SetAnnouncementText(int32 EnemiesLeft)
{
	FString ElimAnnouncementText = FString::Printf(TEXT("You Elimde an enemy! %d enemies left!"), EnemiesLeft);
	if (AnnouncementText)
	{
		AnnouncementText->SetText(FText::FromString(ElimAnnouncementText));
	}
}
