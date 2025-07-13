// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBuffComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Heal(float HealAmount,float HealingTime);

	friend class ABlasterCharacter;
protected:
	virtual void BeginPlay() override;
	void HealRampUp(float DeltaTime);
private:
	TWeakObjectPtr<class ABlasterCharacter> Character;	

	bool bHealing = false;
	float HealingRate = 0.f;
	float AmountToHeal = 0.f;
};
