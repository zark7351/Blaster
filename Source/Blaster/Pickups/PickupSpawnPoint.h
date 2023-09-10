// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class BLASTER_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawnPoint();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickup>> PickupClasses;

	UPROPERTY(EditAnywhere)
	bool bShouldSpawnOnBegin = false;

protected:
	virtual void BeginPlay() override;


	void SpawnPickup();
	void SpawnPickupTimerFinished();
	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);

	UPROPERTY()
	APickup* SpawnedPickup;

private:
	FTimerHandle SpawnPickupTimer;

public:	
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMin;

	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMax;

	UPROPERTY()
	USceneComponent* Root;
};
