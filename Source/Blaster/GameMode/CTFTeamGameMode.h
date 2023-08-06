// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeamGameMode.h"
#include "CTFTeamGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ACTFTeamGameMode : public ATeamGameMode
{
	GENERATED_BODY()
	
public:

	virtual void PlayerEliminated(class ABlasterCharacter* EliminatedPlayer, class ABlasterPlayerController* VictimController, class ABlasterPlayerController* AttackerController) override;
};
