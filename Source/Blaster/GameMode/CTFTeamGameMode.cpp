// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFTeamGameMode.h"

void ACTFTeamGameMode::PlayerEliminated(ABlasterCharacter* EliminatedPlayer, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(EliminatedPlayer, VictimController, AttackerController);

}
