// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeOfLobby.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void AGameModeOfLobby::PostLogin(APlayerController* NewPlayer)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance) return;
	MultiplayerSessionsSubsystem = MultiplayerSessionsSubsystem ==nullptr?GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>(): MultiplayerSessionsSubsystem;
	check(MultiplayerSessionsSubsystem);

	Super::PostLogin(NewPlayer);
	int32 NumbeOfPlayers = GameState.Get()->PlayerArray.Num();

	if (NumbeOfPlayers == 2)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			bUseSeamlessTravel = true;

			FString MatchType = MultiplayerSessionsSubsystem->DesiredMatchType;
			FString Map = MultiplayerSessionsSubsystem->DesiredMap;
			FString Path = FString::Printf(TEXT("/Game/Maps/%s%s?listen"), *Map, *MatchType);
			World->ServerTravel(Path);
		}
	}

}
