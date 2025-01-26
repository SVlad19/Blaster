// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage {
	GENERATED_BODY()

public:
	TObjectPtr<UTexture2D> CrosshairCenter;
	TObjectPtr<UTexture2D> CrosshairLeft;
	TObjectPtr<UTexture2D> CrosshairRight;
	TObjectPtr<UTexture2D> CrosshairTop;
	TObjectPtr<UTexture2D> CrosshairBottom;
	float CrosshairSpread;
};

UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void DrawHUD()override;

	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) {
		HUDPackage = Package;
	}

private:
	
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread);
	
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
};
