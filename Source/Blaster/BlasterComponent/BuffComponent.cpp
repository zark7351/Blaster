// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
	bHealing = true;
	HealingRate = HealAmount / HealingTime;
	AmountToHeal += HealAmount;
}

void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
	bReplenishingShield = true;
	ShieldReplenishRate = ShieldAmount / ReplenishTime;
	ShieldReplenishAmount += ShieldAmount;
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed,float JumpZVelocity)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
	InitialJumpZVelocity = JumpZVelocity;
}

void UBuffComponent::HealRampup(float DeltaTime)
{
	if (!bHealing || Characrer == nullptr || Characrer->IsElimmed()) return;
	const float HealThisFrame = HealingRate * DeltaTime;
	Characrer->SetHealth(FMath::Clamp(Characrer->GetHealth() + HealThisFrame, 0.f, Characrer->GetMaxHealth()));
	Characrer->UpdateHUDHealth();
	AmountToHeal -= HealThisFrame;
	if (AmountToHeal<=0.f || Characrer->GetHealth()>=Characrer->GetMaxHealth())
	{
		bHealing = false;
		AmountToHeal = 0.f;
	}
}

void UBuffComponent::ShieldRampup(float DeltaTime)
{
	if (!bReplenishingShield || Characrer == nullptr || Characrer->IsElimmed()) return;
	const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;
	Characrer->SetShield(FMath::Clamp(Characrer->GetShield() + ReplenishThisFrame, 0.f, Characrer->GetMaxShield()));
	Characrer->UpdateHUDShield();
	ShieldReplenishAmount -= ReplenishThisFrame;
	if (ShieldReplenishAmount <= 0.f || Characrer->GetShield() >= Characrer->GetMaxShield())
	{
		bReplenishingShield = false;
		ShieldReplenishAmount = 0.f;
	}
}


void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HealRampup(DeltaTime);
	ShieldRampup(DeltaTime);
}


void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffJumpZVelocity, float BuffTime)
{
	if (Characrer == nullptr) return;
	Characrer->GetWorldTimerManager().SetTimer(SpeedBuffTimer, this, &UBuffComponent::ResetSpeed,BuffTime);
	if (Characrer->GetCharacterMovement())
	{
		Characrer->GetCharacterMovement()->MaxWalkSpeed = BuffBaseSpeed;
		Characrer->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed;
		Characrer->GetCharacterMovement()->JumpZVelocity = BuffJumpZVelocity;
	}
	MulticastSpeedBuff(BuffBaseSpeed, BuffCrouchSpeed, BuffJumpZVelocity);
}

void UBuffComponent::ResetSpeed()
{
	if (Characrer && Characrer->GetCharacterMovement())
	{
		Characrer->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed;
		Characrer->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed;
		Characrer->GetCharacterMovement()->JumpZVelocity = InitialJumpZVelocity;
	}
	MulticastSpeedBuff(InitialBaseSpeed, InitialCrouchSpeed, InitialJumpZVelocity);
}

void UBuffComponent::MulticastSpeedBuff_Implementation(float BaseSpeed, float CrouchSpeed,float JumpZVelocity)
{
	if (Characrer && Characrer->GetCharacterMovement())
	{
		Characrer->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
		Characrer->GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
		Characrer->GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	}
}

