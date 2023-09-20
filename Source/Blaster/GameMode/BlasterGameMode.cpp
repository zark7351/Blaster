// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/GameState/BlasterGameState.h"
#include "MultiplayerSessionsSubsystem.h"


namespace MatchState
{
	const FName Cooldown = FName("Cooldown");
}

ABlasterGameMode::ABlasterGameMode()
{
	bDelayedStart = true;
}


void ABlasterGameMode::BeginPlay()
{
	Super::BeginPlay();
	LevelStartTime = GetWorld()->GetTimeSeconds();
}

void ABlasterGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();It;++It )
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer)
		{
			BlasterPlayer->OnMatchStateSet(MatchState,bSingleMode,bTeamsMatch);
		}
	}
}

void ABlasterGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MatchState==MatchState::WaitingToStart)
	{
		CountdownTime = WarmupTime - GetWorld()->GetTimeSeconds() + LevelStartTime;
		if (CountdownTime<=0.f)
		{
			StartMatch();	
		}
	}
	else if (MatchState==MatchState::InProgress)
	{
		CountdownTime = WarmupTime + MatchTime - GetWorld()->GetTimeSeconds() + LevelStartTime;
		if (CountdownTime<=0.f)
		{
			SetMatchState(MatchState::Cooldown);
		}
	}
	else if (MatchState==MatchState::Cooldown)
	{
		CountdownTime = WarmupTime + MatchTime + CooldownTime - GetWorld()->GetTimeSeconds() + LevelStartTime;
		if (CountdownTime<=0.f)
		{
			RestartGame();
		}
	}
}

void ABlasterGameMode::PlayerEliminated(ABlasterCharacter* EliminatedPlayer, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackersPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState):nullptr;
	ABlasterPlayerState* VictimsPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	ABlasterGameState* BlasterGamestate = GetGameState<ABlasterGameState>();
	if (AttackersPlayerState && AttackersPlayerState != VictimsPlayerState && BlasterGamestate)
	{
		TArray<ABlasterPlayerState*>PlayerCurrentlyInTheLead;
		for (auto LeadPlayer : BlasterGamestate->TopScoringPlayers)
		{
			PlayerCurrentlyInTheLead.Add(LeadPlayer);
		}
		AttackersPlayerState->AddToScore(1.f);
		BlasterGamestate->UpdateTopScore(AttackersPlayerState);
		if (BlasterGamestate->TopScoringPlayers.Contains(AttackersPlayerState))
		{
			ABlasterCharacter* Leader = Cast<ABlasterCharacter>(AttackersPlayerState->GetPawn());
			if (Leader)
			{
				Leader->MulticastGainTheLead();
			}
		}

		for (auto LeadPlayer : PlayerCurrentlyInTheLead)
		{
			if (!BlasterGamestate->TopScoringPlayers.Contains(LeadPlayer))
			{
				ABlasterCharacter* Loser = Cast<ABlasterCharacter>(LeadPlayer->GetPawn());
				if (Loser)
				{
					Loser->MulticastLostTheLead();
				}
			}
		}
	}
	if (VictimsPlayerState)
	{
		VictimsPlayerState->AddToDefeats(1);
		
	}
	if (EliminatedPlayer)
	{
		EliminatedPlayer->Elim(false);
	}
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(*It);
		if (BlasterPlayer && AttackersPlayerState && VictimsPlayerState)
		{
			BlasterPlayer->BroadcastElim(AttackersPlayerState, VictimsPlayerState);
		}
	}
}

void ABlasterGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection= FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void ABlasterGameMode::PlayerLeftGame(ABlasterPlayerState* PlayerLeaving)
{
	if (PlayerLeaving == nullptr) return;
	ABlasterGameState* BlasterGamesState = GetGameState<ABlasterGameState>();
	if (BlasterGamesState && BlasterGamesState->TopScoringPlayers.Contains(PlayerLeaving))
	{
		BlasterGamesState->TopScoringPlayers.Remove(PlayerLeaving);
	}
	ABlasterCharacter* CharacterLeaving = Cast<ABlasterCharacter>(PlayerLeaving->GetPawn());
	if (CharacterLeaving)
	{
		CharacterLeaving->Elim(true);
	}
}

float ABlasterGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	return BaseDamage;
}
