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
	FLinearColor CrosshairsColor;
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

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> CharacterOverlayClass;

	TObjectPtr<class UCharacterOverlay> CharacterOverlay;

protected:

	virtual void BeginPlay() override;
	void AddCharacterOverlay();

private:
	
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);
	
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
};
