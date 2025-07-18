// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BLASTER_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	APickup();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed()override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap
	(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:
	void BindOverlapTimerFinished();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> OverlapSphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> PickupSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> PickupMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UNiagaraComponent> PickupEffectComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> PickupEffect;

	FTimerHandle BindOverlapTimer;

	float BindOverlapTime = 0.25f;

};
