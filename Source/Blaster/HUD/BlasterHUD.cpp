// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterHUD.h"

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
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread= FVector2D(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread = FVector2D(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread = FVector2D(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter,Spread);
		}
		if (HUDPackage.CrosshairsButtom)
		{
			FVector2D Spread = FVector2D(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsButtom, ViewportCenter,Spread);
		}
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
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
		FLinearColor::White,
		EBlendMode::BLEND_Translucent
	);
}
