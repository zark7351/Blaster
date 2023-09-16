// Fill out your copyright notice in the Description page of Project Settings.


#include "AISpawnPoint.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster//BlasterComponent/CombatComponent.h"

AAISpawnPoint::AAISpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SetRootComponent(SpawnPoint);
}

void AAISpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	SpawnAI();
}

void AAISpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawnPoint::SpawnAI()
{
	UWorld* World = GetWorld();
	if (World)
	{
		int32 CharacterSelection = FMath::RandRange(0, SpawnClasses.Num() - 1);
		ABlasterCharacter* AI = World->SpawnActorDeferred<ABlasterCharacter>(SpawnClasses[CharacterSelection], GetTransform(),nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		if (AI)
		{
			int32 WeaponSelection = FMath::RandRange(0, DefaultWeaponClasses.Num() - 1);
			AI->SetDefaultWeaponClass(DefaultWeaponClasses[WeaponSelection]);
			AI->FinishSpawning(GetTransform());
			AI->SpawnDefaultController();
		}
	}
}

