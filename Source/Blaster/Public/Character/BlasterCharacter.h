// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterTypes/TurningInPlace.h"
#include "BlasterCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();
	void SetOverlappingWeapon(class AWeapon* Weapon);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	virtual void PostInitializeComponents()override;
	virtual void Jump() override;

	bool IsWeaponEquipped();
	bool IsAiming();

	FORCEINLINE float GetAO_Yaw()const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch()const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace()const { return TurningInPlace; }
	FORCEINLINE UCameraComponent* GetFollowCamera()const { return FollowCamera; }

	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;

	void TurnInPlace(float DeltaTime);

	void PlayFireMontage(bool bAiming);


protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	void AimOffset(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr <class UInputDataAsset > InputDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCombatComponent> Combat;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<class UAnimMontage> FireWeaponMontage;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jumping(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Crouching(const FInputActionValue& Value);
	void Aiming(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
};
