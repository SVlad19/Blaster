// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupSpawnPoint.generated.h"

UCLASS()
class BLASTER_API APickupSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupSpawnPoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	void SpawnPickup();
	void SpawnPickupTimerFinished();
	UFUNCTION()
	void StartSpawnPickupTimer(AActor* DestroyedActor);

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APickup>> PickupClasses;

private:
	FTimerHandle SpawnPickupTimer;

	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMin;
	UPROPERTY(EditAnywhere)
	float SpawnPickupTimeMax;

	TObjectPtr<APickup> SpawnedPickup;
};
