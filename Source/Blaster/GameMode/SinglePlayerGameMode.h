// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterGameMode.h"
#include "SinglePlayerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ASinglePlayerGameMode : public ABlasterGameMode
{
	GENERATED_BODY()
	
public:

	virtual void PlayerEliminated(class ABlasterCharacter* EliminatedPlayer, class ABlasterPlayerController* VictimController, class ABlasterPlayerController* AttackerController);

protected:
	virtual void BeginPlay()override;

private:
	int32 CurrentEnemies = 0;
	
public:
	FORCEINLINE int32 GetCurrentEnemies()const { return CurrentEnemies; }
};
