// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "WeaponTypes.h"
#include "Blaster/BlasterComponent/LagCompensationComponent.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();

	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
	if (MuzzleFlashSocket)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector Start = SocketTransform.GetLocation();
		FVector End = Start + (HitTarget - Start) * 1.25f;

		FHitResult FireHit;
		WeaponTranceHit(Start, HitTarget, FireHit);
		if (FireHit.bBlockingHit)
		{
			ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(FireHit.GetActor());
			if (BlasterCharacter && InstigatorController)
			{
				//这里的逻辑我感觉讲的有问题,没有考虑到服务器本地控制且bUseServerSideRewind为true的情况
				//if (HasAuthority() && !bUseServerSideRewind)	
				//{
				//	UGameplayStatics::ApplyDamage(
				//		BlasterCharacter,
				//		Damage,
				//		InstigatorController,
				//		this,
				//		UDamageType::StaticClass()
				//	);
				//}
				if (HasAuthority())
				{
					if (OwnerPawn->IsLocallyControlled())
					{
						UGameplayStatics::ApplyDamage(
							BlasterCharacter,
							Damage,
							InstigatorController,
							this,
							UDamageType::StaticClass()
						);
					}
					else if (!bUseServerSideRewind)
					{
						UGameplayStatics::ApplyDamage(
							BlasterCharacter,
							Damage,
							InstigatorController,
							this,
							UDamageType::StaticClass()
						);
					}
				}
				if(!HasAuthority() && bUseServerSideRewind && OwnerPawn->IsLocallyControlled())
				{
					BlasterOwnerCharacter=BlasterOwnerCharacter==nullptr?Cast<ABlasterCharacter>(OwnerPawn):BlasterOwnerCharacter;
					BlasterOwnerController=BlasterOwnerController==nullptr?Cast<ABlasterPlayerController>(InstigatorController) :BlasterOwnerController;
					if (BlasterOwnerCharacter && BlasterOwnerController && BlasterOwnerCharacter->GetLagCompensation())
					{
						BlasterCharacter->GetLagCompensation()->ServerScoreRequest(
							BlasterCharacter,
							Start,
							HitTarget,
							BlasterOwnerController->GetServerTime() - BlasterOwnerController->SingleTripTime,
							this
						);
					}
				}
			}
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticles,
					FireHit.ImpactPoint,
					FireHit.ImpactNormal.Rotation()
				);
			}
			if (HitSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					HitSound,
					FireHit.ImpactPoint
				);
			}
		}
		if (ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				ImpactParticles,
				FireHit.ImpactPoint,
				FireHit.ImpactNormal.Rotation()
			);
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				HitSound,
				FireHit.ImpactPoint
			);
		}
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				MuzzleFlash,
				SocketTransform
			);
		}
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				FireSound,
				GetActorLocation()
			);
		}
	}
}


void AHitScanWeapon::WeaponTranceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FVector End = TraceStart + (HitTarget - TraceStart) * 1.25f;
		World->LineTraceSingleByChannel(
			OutHit,
			TraceStart,
			End,
			ECollisionChannel::ECC_Visibility
		);
		FVector BeamEnd = End;
		if (OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}

		if (BeamParticles)
		{
			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				World,
				BeamParticles,
				TraceStart,
				FRotator::ZeroRotator,
				true
			);
			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
		}
	}

}
