// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	void StartDestroyTimer();
	void DestroyTimerFinished();
	void ExplodeDamage();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NoramlImpulse, const FHitResult& Hit);

	void SpawnTrailSystem();

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USoundCue> ImpactSound;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> TrailSystem;

	TWeakObjectPtr<class UNiagaraComponent> TrailSystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;

	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 500.f;

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> Tracer;	
	
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UParticleSystemComponent> TracerComponent;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;

};
