// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameModeOfLobby.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AGameModeOfLobby : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer)override;
};
