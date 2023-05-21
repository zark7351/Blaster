// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void Reload();
	int32 AmountToReload();
	UFUNCTION()
	void OnRep_EquippedWeapon();
	UFUNCTION(Server, Reliable)
	void ServerReload();
	void HandleReload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION()
	void FireButtonPressed(bool bPressed);

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAming);
	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAming);

	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitResult);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitResult);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* PlayerController;
	UPROPERTY()
	class ABlasterHUD* HUD;

	UPROPERTY(ReplicatedUsing=OnRep_EquippedWeapon)
	class AWeapon* EquippedWeapon;
	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;
	bool bFireButtonPressed;
	
	/*
	* HUD and Crosshairs
	*/
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	FVector HitTarget;
	FHUDPackage HUDPackage;
	
	/*
	* Aiming and FOV
	*/

	float DefaultFOV;
	UPROPERTY(EditAnywhere,Category = Combat)
	float ZoomedFOV = 30.f;
	float CurrentFOV;
	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/*
	* Automatic Fire
	*/

	FTimerHandle FireTimer;


	bool bCanFire=true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	//Carried ammo for the currendly-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo=30;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();
};
