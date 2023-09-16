// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AISpawnPoint.generated.h"

UCLASS()
class BLASTER_API AAISpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AAISpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ABlasterCharacter>>SpawnClasses;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AWeapon>> DefaultWeaponClasses;
	void SpawnAI();

private:
	USceneComponent* SpawnPoint;
};
