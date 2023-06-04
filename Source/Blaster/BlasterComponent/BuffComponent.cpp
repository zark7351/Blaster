// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"
#include "Blaster/Character/BlasterCharacter.h"

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


void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	HealRampup(DeltaTime);
}

