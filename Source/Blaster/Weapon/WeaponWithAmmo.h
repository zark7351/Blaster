// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponWithAmmo.generated.h"

UCLASS()
class BLASTER_API AWeaponWithAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponWithAmmo();

protected:
	virtual void BeginPlay() override;
	void SpawnWeaponWithAmmo();

	class AWeapon* SpawnedWeapon;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AWeapon>>WeaponClasses;

};
