// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"
#include "GameFrameWork/PlayerController.h"
#include "CharacterOverlay.h"

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		FVector2D ViewportCenter = ViewportSize / 2.f;

		float SpreadScaled= CrosshairSpreadMax* HUDPackage.CrosshairSpread;
		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread=FVector2D(0.f,0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter,Spread,HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread= FVector2D(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter,Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread = FVector2D(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter,Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread = FVector2D(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter,Spread, HUDPackage.CrosshairColor);
		}
		if (HUDPackage.CrosshairsButtom)
		{
			FVector2D Spread = FVector2D(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsButtom, ViewportCenter,Spread, HUDPackage.CrosshairColor);
		}
	}
}

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCharacterOverlay();
}

void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread,FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight =Texture->GetSizeY();
	const FVector2D CrosshairDrawPosition(ViewportCenter.X - TextureWidth / 2.f + Spread.X, ViewportCenter.Y - TextureHeight / 2.f + Spread.Y);
	DrawTexture(
		Texture,
		CrosshairDrawPosition.X,
		CrosshairDrawPosition.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor,
		EBlendMode::BLEND_Translucent
	);
}
