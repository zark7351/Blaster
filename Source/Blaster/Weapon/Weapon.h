// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponTypes.h"
#include "Blaster/BlasterTypes/Team.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState :uint8
{
	EWS_Initial UMETA(DisplayName="Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_EquippedSecondary UMETA(DisplayName = "EquippedSecondary"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMax"),
};

UENUM(BlueprintType)
enum class EFireType :uint8
{
	EFT_HitScan UMETA(DisplayName = "Hit Scan Weapon"),
	EFT_Projectile UMETA(DisplayName = "Projectile Weapon"),
	EFT_Shotgun UMETA(DisplayName = "Shotgun Weapon"),

	EFT_MAX UMETA(DisplayName = "DefaultMax"),
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRep_Owner() override;
	void SetHUDAmmo();
	void ShowPickupWidget(bool bShowWidget);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APickup>AmmoPickupClass;


	//Textures fot the weapon crosshairs

	UPROPERTY(EditAnywhere)
		class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere)
		class UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere)
		class UTexture2D* CrosshairsRight;

	UPROPERTY(EditAnywhere)
		class UTexture2D* CrosshairsTop;

	UPROPERTY(EditAnywhere)
		class UTexture2D* CrosshairsButtom;

	virtual void Dropped();
	void AddAmmo(int32 AmmoToAdd);

	FVector TranceEndWithScatter(const FVector& HitTarget);

	/*
	* Automatic Fire
	*/
	UPROPERTY(EditAnywhere, Category = Combat,BlueprintReadWrite)
		bool bAutomatic = true;

	UPROPERTY(EditAnywhere, Category = Combat)
		float FireDelay = 0.15f;

	UPROPERTY(EditAnywhere)
	class USoundCue* EquippedSound;

	/*
	* Enable or disable custom depth
	*/

	void EnableCustomDepth(bool bEnable);
	
	// Default weapon
	bool bDestroyWeapon = false;

	UPROPERTY(EditAnywhere)
	EFireType FireType;

protected:

	virtual void BeginPlay() override;
	virtual void OnWeaponStateSet();
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnEquippedSecondary();

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
		void OnSphereEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex
		);

	/*
	* Trace end for scatter
	*/

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float DistanceToSphere = 800.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	float SphereRadius = 75.f;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;

	UPROPERTY(Replicated,EditAnywhere)
	bool bUseServerSideRewind=false;

	UPROPERTY()
	class ABlasterCharacter* BlasterOwnerCharacter;
	UPROPERTY()
	class ABlasterPlayerController* BlasterOwnerController;

	UFUNCTION()
	void OnPingTooHigh(bool bPingTooHigh);

private:
	UPROPERTY(VisibleAnywhere,Category="Weapon Properties")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing= OnRep_WeaponState,VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UFUNCTION()
	void OnRep_WeaponState();
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UWidgetComponent* PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;

	/*
	*Zoomed FOV While aiming
	*/

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float ZoomedFOV=30.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float ZoomInterpSpeed=20.f;

	UPROPERTY(EditAnywhere)
	int32 Ammo;

	UFUNCTION(Client,Reliable)
	void ClientUpdateAmmo(int32 ServerAmmo);
	UFUNCTION(Client, Reliable)
	void ClientAddAmmo(int32 AmmoToAdd);

	void SpendRound();
	
	//The number of unprocessed server request for Ammo.
	//Incremented in SpendRound and decremented in ClientUpdateAmmo.
	int32 Sequence = 0;

	UPROPERTY(EditAnywhere)
	int32 MagCapacity;

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	ETeam Team;

		
public:	
	void SetWeaponState(EWeaponState State);
	virtual void Fire(const FVector& HitTarget);

	FORCEINLINE USphereComponent* GetAreaSphere()const { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh()const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV()const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed()const { return ZoomInterpSpeed; }
	bool IsEmpty();
	bool IsFull();
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EWeaponType GetWeaponType()const { return WeaponType; }
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagCapacity() const { return MagCapacity; }
	FORCEINLINE float GetDamage() const { return Damage; }
	FORCEINLINE float GetHeadShotDamage() const { return HeadShotDamage; }
	FORCEINLINE UWidgetComponent* GetPickupWidget()const { return PickupWidget; }
	FORCEINLINE ETeam GetTeam()const { return Team; }

	UPROPERTY(EditAnywhere, Category = "Weapon Scatter")
	bool bUseScatter = false;	
};

