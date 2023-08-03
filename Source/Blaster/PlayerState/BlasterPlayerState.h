// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	/**
	*	Replication Notifies
	*/
	virtual void OnRep_Score() override;
	UFUNCTION()
	void On_Rep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatAmount);


private:
	UPROPERTY()
	class  ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing = On_Rep_Defeats)
	int32 Defeats;

	UPROPERTY(Replicated)
	ETeam Team = ETeam::ET_NoTeam;

	UFUNCTION()
	void OnRep_Team();

public:
	FORCEINLINE ETeam GetTeam()const { return Team; }
	void SetTeam(ETeam TeamToSet);
};
