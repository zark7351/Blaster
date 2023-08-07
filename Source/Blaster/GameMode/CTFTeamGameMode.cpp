// Fill out your copyright notice in the Description page of Project Settings.


#include "CTFTeamGameMode.h"
#include "Blaster/Weapon/Flag.h"
#include "Blaster/CaptureTheFlag/FlagZone.h"
#include "Blaster/GameState/BlasterGameState.h"

void ACTFTeamGameMode::PlayerEliminated(ABlasterCharacter* EliminatedPlayer, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterGameMode::PlayerEliminated(EliminatedPlayer, VictimController, AttackerController);

}

void ACTFTeamGameMode::FlagCaptured(AFlag* Flag, AFlagZone* Zone)
{
	bool bValidCapture = Flag->GetTeam() != Zone->Team;
	ABlasterGameState* BGameState = Cast<ABlasterGameState>(GameState);
	if (BGameState)
	{
		if (Zone->Team==ETeam::ET_BlueTeam)
		{
			BGameState->BlueTeamScores();
		}
		if (Zone->Team==ETeam::ET_RedTeam)
		{
			BGameState->RedTeamScores();
		}
	}
}
