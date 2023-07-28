// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTartget = HitTarget - SocketTransform.GetLocation();
		FRotator ToTargetRot = ToTartget.Rotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile = nullptr;
		if (bUseServerSideRewind)
		{
			if (InstigatorPawn->HasAuthority()) //server
			{
				if (InstigatorPawn->IsLocallyControlled()) // server ,host -use replicated projectile
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass,SocketTransform.GetLocation(),ToTargetRot,SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
				}
				else // server, not locally controlled -spawn non-replicated projectile, no SSR
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), ToTargetRot, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = false;
				}
			}
			else // client, using SSR
			{
				if (InstigatorPawn->IsLocallyControlled())
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(), ToTargetRot, SpawnParams);
					SpawnedProjectile->bUseServerSideRewind = true;
					SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					SpawnedProjectile->InitialSpeed = SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
				}
			}
		else
		{

		}
		World->SpawnActor<AProjectile>
			(
				ProjectileClass,
				SocketTransform.GetLocation(),
				ToTargetRot,
				SpawnParams
			);
	}
}
