// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUD/BlasterHUD.h"
#include "Weapon/WeaponTypes.h"
#include "BlasterTypes/CombatState.h"
#include "CombatComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();
	
	void EquipWeapon(class AWeapon* WeaponToEquip);
	void Reload();
	void SwapWeapons();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	void FireButtonPressed(bool bPressed);

	UFUNCTION(BlueprintCallable)
	void ShotgunShellReload();

	void JumpToShotgunEnd();

	UFUNCTION(BlueprintCallable)
	void ThrowGrenadeFinished();

	UFUNCTION(BlueprintCallable)
	void LaunchGrenade();

	UFUNCTION(Server,Reliable)
	void ServerLaunchGrenade(const FVector_NetQuantize Target);

	void PickupAmmo(EWeaponType WeaponType, int32 AmmoAmount);

	FORCEINLINE int32 GetGrenades()const { return Grenades; }
	bool ShouldSwapWeapons();

	friend class ABlasterCharacter;

protected:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	void SetAiming(bool bIsAiming);
	void DropEquippedWeapon();
	void Fire();
	void AttachActorToRightHand(AActor* ActorToAttach);
	void AttachActorToLeftHand(AActor* ActorToAttach);
	void AttachActorToBackpack(AActor* ActorToAttach);
	void UpdateCarriedAmmo();
	void PlayEquipWeaponSound(AWeapon* WeaponToEquip);
	void ReloadEmptyWeapon();
	void ShowAttachedGrenade(bool bShowGrenade);
	void EquipPrimaryWeapon(AWeapon* WeaponToEquip);
	void EquipSecondaryWeapon(AWeapon* WeaponToEquip);

	UFUNCTION(Server,Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);	
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);	
	
	UFUNCTION(Server, Reliable)
	void ServerReload();
	
	void HandleReload();

	int32 AmountToReload();

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UFUNCTION()
	void OnRep_SecondaryWeapon();

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	void ThrowGrenade();

	UFUNCTION(Server,Reliable)
	void ServerThrowGreande();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> GrenadeClass;

private:

	TWeakObjectPtr<class ABlasterCharacter> Character;
	TWeakObjectPtr<class ABlasterPlayerController> Controller;
	TWeakObjectPtr<class ABlasterHUD> HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TWeakObjectPtr<class AWeapon> EquippedWeapon;

	UPROPERTY(ReplicatedUsing = OnRep_SecondaryWeapon)
	TWeakObjectPtr<class AWeapon> SecondaryWeapon;
	

	UPROPERTY(Replicated)
	bool bAiming;

	bool bFireButtonPressed;
	
	FVector HitTarget;

	/**
	*  HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	FHUDPackage HUDPackage;

	/**
	* Aiming and FOV
	*/

	void InterpFOV(float DeltaTime);

	// Field of view when not aiming; set to the camera`s base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ZoomInterpSpeed= 20.f;

	/*
	* Automatic fire
	*/

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire()const;

	FTimerHandle FireTimer;

	bool bCanFire = true;

	/*
	* Carried ammo
	*/

	void InitializeCarriedAmmo();

	UFUNCTION()
	void OnRep_CarriedAmmo();

	UFUNCTION()
	void OnRep_CombatState();

	UFUNCTION()
	void OnRep_Grenades();

	void UpdateAmmoValues();
	void UpdateShotgunAmmoValues();
	void UpdateHUDGrenades();

	// Carried ammo for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 MaxCarriedAmmo = 500;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;

	UPROPERTY(EditAnywhere)
	int32 StartingRocketAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingPistolAmmo = 0;	
	
	UPROPERTY(EditAnywhere)
	int32 StartingSMGAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingShotgunAmmo = 0;	
	
	UPROPERTY(EditAnywhere)
	int32 StartingSniperAmmo = 0;

	UPROPERTY(EditAnywhere)
	int32 StartingGrenadeLauncherAmmo = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Grenades)
	int32 Grenades = 4;

	UPROPERTY(EditAnywhere)
	int32 MaxGrenades = 4;

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

};
