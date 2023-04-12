// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	if(!HasAuthority()||!GetOwner()) return;
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTartget = HitTarget - SocketTransform.GetLocation();
		FRotator ToTargetRot = ToTartget.Rotation();
		if (ProjectileClass && InstigatorPawn)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner= GetOwner();
				SpawnParams.Instigator = InstigatorPawn;
				World->SpawnActor<AProjectile>
					(
						ProjectileClass,
						SocketTransform.GetLocation(),
						ToTargetRot,
						SpawnParams
					);
			}
		}

	}
}
