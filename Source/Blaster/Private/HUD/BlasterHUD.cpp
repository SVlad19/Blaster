// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BlasterHUD.h"
#include "GameFramework/PlayerController.h"
#include "HUD/CharacterOverlay.h"

void ABlasterHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();
}

void ABlasterHUD::AddCharacterOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass) {
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		CharacterOverlay->AddToViewport();
	}
}

void ABlasterHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;

	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter{ ViewportSize.X / 2.f,ViewportSize.Y / 2.f };

	float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

	if (HUDPackage.CrosshairCenter) {
		FVector2D Spread(0.f, 0.f);
		DrawCrosshair(HUDPackage.CrosshairCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
	}	
	if (HUDPackage.CrosshairLeft) {
		FVector2D Spread(-SpreadScaled, 0.f);
		DrawCrosshair(HUDPackage.CrosshairLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
	}	
	if (HUDPackage.CrosshairRight) {
		FVector2D Spread(SpreadScaled, 0.f);
		DrawCrosshair(HUDPackage.CrosshairRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
	}	
	if (HUDPackage.CrosshairTop) {
		FVector2D Spread(0.f, -SpreadScaled);
		DrawCrosshair(HUDPackage.CrosshairTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
	}
	if (HUDPackage.CrosshairBottom) {
		FVector2D Spread(0.f, SpreadScaled);
		DrawCrosshair(HUDPackage.CrosshairBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
	}
}

void ABlasterHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();

	const FVector2D TextureDrawPoint{
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y,
	};

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
	);

}
