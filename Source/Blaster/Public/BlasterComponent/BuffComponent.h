// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Heal(float HealAmount, float HealingTime);
	void BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime);
	void BuffJump(float BuffJumpVelocity, float BuffTime);
	void SetInitialSpeeds(float BaseSpeed, float CrouchSpeed);
	void SetInitialJumpVelocity(float Velocity);

	friend class ABlasterCharacter;
protected:
	virtual void BeginPlay() override;
	void HealRampUp(float DeltaTime);
private:
	TWeakObjectPtr<class ABlasterCharacter> Character;

	/*
	* Heal buff
	*/

	bool bHealing = false;
	float HealingRate = 0.f;
	float AmountToHeal = 0.f;

	/*
	* Speed buff
	*/

	void ResetSpeeds();
	UFUNCTION(NetMulticast,Reliable)
	void MulticastSpeedBuff(float BaseSpeed, float CrouchSpeed);

	FTimerHandle SpeedBuffTimer;
	float InitialBaseSpeed;
	float InitialCrouchSpeed;

	/*
	* Jump buff
	*/

	void ResetJump();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastJumpBuff(float JumpVelocity);

	FTimerHandle JumpBuffTimer;
	float InitialJumpVelocity;


};
