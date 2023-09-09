// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHeadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverHeadWidget::SetPlayerName(FString NameToDisPlay)
{
	if (PlayerName)
	{
		PlayerName->SetText(FText::FromString(NameToDisPlay));
	}
}

void UOverHeadWidget::ShowPLayerNetRole(APawn* InPawn)
{
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ROLE_None:
		Role = FString("None");
		break;
	case ROLE_SimulatedProxy:
		Role = FString("SimulatedProxy");
		break;
	case ROLE_AutonomousProxy:
		Role = FString("AutonomousProxy");
		break;
	case ROLE_Authority:
		Role = FString("Authority");
		break;
	case ROLE_MAX:
		break;
	default:
		break;
	}
	FString RemoteString = FString::Printf(TEXT("Remote Role %s"), *Role);
	SetDisplayText(RemoteString);
}

void UOverHeadWidget::ShowPLayerName(APawn* InPawn)
{
	FString Name;
	APlayerState* PlayerState = InPawn->GetPlayerState();
	if (PlayerState)
	{
		Name=PlayerState->GetPlayerName();
		SetPlayerName(Name);
	}
}

void UOverHeadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

//void UOverHeadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
//{
//	RemoveFromParent();
//	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
//}
