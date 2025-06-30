// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& HitTarget)override;
	
private:

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> ImpactPaticles;	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> BeamPaticles;	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> MuzzleFlash;	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> FireSound;	
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> HitSound;
};
