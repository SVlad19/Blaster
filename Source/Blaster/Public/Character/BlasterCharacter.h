// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Interfaces/InteractWithCrosshairsInterface.h"
#include "Components/TimelineComponent.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	void SetOverlappingWeapon(class AWeapon* Weapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	void RotateInPlace(float DeltaTime);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	virtual void PostInitializeComponents()override;
	virtual void Jump() override;
	virtual void OnRep_ReplicatedMovement() override;
	virtual void Destroyed() override;

	bool IsWeaponEquipped();
	bool IsAiming();

	FORCEINLINE float GetAO_Yaw()const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch()const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace()const { return TurningInPlace; }
	FORCEINLINE UCameraComponent* GetFollowCamera()const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone()const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth () const { return Health; }
	FORCEINLINE void SetHealth (float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth () const { return MaxHealth; }
	FORCEINLINE bool GetDisableGameplay()const { return bDisableGameplay; }
	FORCEINLINE class UCombatComponent* GetCombat()const { return Combat; }
	FORCEINLINE class UBuffComponent* GetBuff()const { return Buff; }
	FORCEINLINE UAnimMontage* GetReloadMontage()const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade()const { return AttachedGrenade; }
	ECombatState GetCombatState()const;

	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;

	void TurnInPlace(float DeltaTime);

	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayHitReatMontage();
	void PlayElimMontage();
	void PlayThrowGrenadeMontage();
	void Elim();
	void UpdateHUDHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;


protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController);

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	void AimOffset(float DeltaTime);
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	
	// Poll for any relevant classes and initialize our HUD
	void PollInit();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr <class UInputDataAsset > InputDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCombatComponent> Combat;	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBuffComponent> Buff;

	/**
	* Animation Montages
	*/

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> FireWeaponMontage;


	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> ReloadMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> ElimMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> ThrowGrenadeMontage;

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;

	/*
	* Grenade
	*/

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AttachedGrenade;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jumping(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Crouching(const FInputActionValue& Value);
	void Aiming(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);
	void Reload(const FInputActionValue& Value);
	void ThrowGrenade(const FInputActionValue& Value);

	void HideCameraIfCharacterClose();
	float CalculateSpeed();

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;

	/**
	* Player Health
	*/

	UFUNCTION()
	void OnRep_Health(float LastHealth);
	void ElimTimerFinished();

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Player Stats")
	float ElimDelay = 3.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere ,Category = "Player Stats")
	float Health = 100.f;

	TWeakObjectPtr<class ABlasterPlayerController> BlasterPlayerController;

	bool bElimmed = false;
	FTimerHandle ElimTimer;

	/**
	* Dissolve Effect
	*/

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);

	void StartDissolve();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimeline;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DissolveCurve;

	// Dynamic instance that we can change at runtime
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;

	// Material instance set on the Blueprint, used with dynamic material instance
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	FOnTimelineFloat DissolveTrack;

	/**
	*	Elim bot
	*/

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UParticleSystemComponent> ElimBotComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> ElimBotSound;

	TWeakObjectPtr<class ABlasterPlayerState> BlasterPlayerState;
};
