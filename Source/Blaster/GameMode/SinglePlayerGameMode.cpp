// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayerGameMode.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blaster//AISpawnPoint.h"

void ASinglePlayerGameMode::PlayerEliminated(ABlasterCharacter* EliminatedPlayer, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController)
{
	ABlasterPlayerState* AttackersPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	ABlasterPlayerState* VictimsPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;
	if (AttackersPlayerState && AttackersPlayerState != VictimsPlayerState)
	{
		AttackersPlayerState->AddToScore(1.f);
	}
	if (EliminatedPlayer)
	{
		if (EliminatedPlayer->IsAI)
		{
			CurrentEnemies--;
		}
		EliminatedPlayer->Elim(false);
	}
	ABlasterPlayerController* BlasterPlayer = Cast<ABlasterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (BlasterPlayer)
	{
		BlasterPlayer->BroadcastElim(AttackersPlayerState, nullptr);
	}

	if (CurrentEnemies <= 0)
	{
		SetMatchState(MatchState::Cooldown);
	}
}

void ASinglePlayerGameMode::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> OutActors;
	TSubclassOf<AAISpawnPoint>Class = AAISpawnPoint::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(this,Class,OutActors);
	CurrentEnemies = OutActors.Num();
}
