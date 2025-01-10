// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();
	
	void EquipWeapon(class AWeapon* WeaponToEquip);

	friend class ABlasterCharacter;

protected:

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server,Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

private:

	TWeakObjectPtr<class ABlasterCharacter> Character;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	TWeakObjectPtr<class AWeapon> EquippedWeapon;
	
	UPROPERTY(Replicated)
	bool bAiming;

};
