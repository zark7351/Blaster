// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeOfLobby.h"
#include "GameFramework/GameStateBase.h"

void AGameModeOfLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	int32 NumbeOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumbeOfPlayers==2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Map/BlasterMap?listen"));
		}
	}
}
