// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_Max UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AWeapon();
	void ShowPickupWidget(bool bShowWidget);
	void SetWeaponState(EWeaponState State);

	FORCEINLINE class USphereComponent* GetAreaSphere()const { return AreaSphere.Get(); }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh()const { return WeaponMesh; }
	FORCEINLINE float GetZoomedFOV()const { return ZoomedFOV; }
	FORCEINLINE float GetZoomInterpSpeed()const { return ZoomInterpSpeed; }

	virtual void Fire(const FVector& HitTarget);

	/*
	* Textures for the Weapon crosshairs
	*/

	UPROPERTY(EditAnywhere, Category = "Croshairs")
	TObjectPtr<UTexture2D> CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Croshairs")
	TObjectPtr<UTexture2D> CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Croshairs")
	TObjectPtr<UTexture2D> CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Croshairs")
	TObjectPtr<UTexture2D> CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Croshairs")
	TObjectPtr<UTexture2D> CrosshairsBottom;

	/**
	* Zoomed FOV while aiming
	*/

	UPROPERTY(EditAnywhere)
	float ZoomedFOV = 30.f;

	UPROPERTY(EditAnywhere)
	float ZoomInterpSpeed = 20.f;

	/** 
	* Automatic Fire
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bAutomatic = true;

protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> PickupWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	TObjectPtr<class UAnimationAsset> FireAnimation;

private:

	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState)
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACasing> CasingClass;
};
