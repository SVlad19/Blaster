// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "ProjectileRocket.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectileRocket();
	virtual void Destroyed()override;

protected:
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NoramlImpulse, const FHitResult& Hit) override;
	virtual void BeginPlay() override;
	void DestroyTimerFinished();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> TrailSystem;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> ProjectileLoop;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundAttenuation> LoopingSoundAttenuation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class URocketMovementComponent> RocketMovementComponent;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RocketMesh;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;

	TWeakObjectPtr<class UNiagaraComponent> TrailSystemComponent;
	TWeakObjectPtr<class UAudioComponent> ProjectileLoopComponent;

};

