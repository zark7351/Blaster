// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeOfLobby.h"
#include "GameFramework/GameStateBase.h"
#include "MultiplayerSessionsSubsystem.h"

void AGameModeOfLobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	int32 NumbeOfPlayers = GameState.Get()->PlayerArray.Num();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UMultiplayerSessionsSubsystem* SubSystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		check(SubSystem);

		if (NumbeOfPlayers == 6)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				bUseSeamlessTravel = true;

				FString MatchType = SubSystem->DesiredMatchType;
				FString Map = SubSystem->DesiredMap;
				FString Path = FString::Printf(TEXT("/Game/Maps/%s%s?listen"), *Map, *MatchType);
				World->ServerTravel(Path);
			}
		}
	}

}
