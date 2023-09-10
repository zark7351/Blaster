// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponWithAmmo.h"
#include "Blaster/Pickups/PickupSpawnPoint.h"
#include "Weapon.h"
#include "Blaster/Pickups/Pickup.h"
#include "Kismet/GameplayStatics.h"


AWeaponWithAmmo::AWeaponWithAmmo()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWeaponWithAmmo::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeaponWithAmmo();
}

void AWeaponWithAmmo::SpawnWeaponWithAmmo()
{
	if (WeaponClasses.Num()>0)
	{
		int32 Selection = FMath::RandRange(0, WeaponClasses.Num() - 1);
		SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClasses[Selection], GetActorTransform());
		if (SpawnedWeapon && SpawnedWeapon->AmmoPickupClass->StaticClass())
		{
			APickupSpawnPoint* Ammo = GetWorld()->SpawnActorDeferred<APickupSpawnPoint>(APickupSpawnPoint::StaticClass(), GetTransform());
			if (Ammo)
			{
				Ammo->bShouldSpawnOnBegin = true;
				Ammo->SpawnPickupTimeMin = 10.f;
				Ammo->SpawnPickupTimeMax = 100.f;
				Ammo->PickupClasses.Add(SpawnedWeapon->AmmoPickupClass);
				FTransform Trans = FTransform(FRotator(), GetActorLocation() + FVector(200.f, 0.f, 0.f), FVector(1.f, 1.f, 1.f));
				Ammo->FinishSpawning(Trans);
				Ammo->SetActorTransform(Trans);
			}
		}
	}
}

void AWeaponWithAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

