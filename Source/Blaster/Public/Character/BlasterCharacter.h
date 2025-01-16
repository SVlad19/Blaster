// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterTypes/TurningInPlace.h"
#include "BlasterCharacter.generated.h"



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

	bool IsWeaponEquipped();
	bool IsAiming();

	FORCEINLINE float GetAO_Yaw()const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch()const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace()const { return TurningInPlace; }

	void TurnInPlace(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	void AimOffset(float DeltaTime);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TObjectPtr <class UInputDataAsset > InputDataAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCombatComponent> Combat;

private:
	void Move(const struct FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jumping(const FInputActionValue& Value);
	void Equip(const FInputActionValue& Value);
	void Crouching(const FInputActionValue& Value);
	void Aiming(const FInputActionValue& Value);

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	TObjectPtr<AWeapon> OverlappingWeapon;

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	ETurningInPlace TurningInPlace;
};
