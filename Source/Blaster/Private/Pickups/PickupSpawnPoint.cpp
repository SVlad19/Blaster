// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/PickupSpawnPoint.h"
#include "Pickups/Pickup.h"

APickupSpawnPoint::APickupSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void APickupSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	StartSpawnPickupTimer(nullptr);
}

void APickupSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupSpawnPoint::SpawnPickup()
{
	int32 NumPickupClasses = PickupClasses.Num();
	if (NumPickupClasses > 0) {
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);
		SpawnedPickup = GetWorld()->SpawnActor<APickup>(PickupClasses[Selection], GetActorTransform());
		
		if (HasAuthority() && SpawnedPickup) {
			SpawnedPickup->OnDestroyed.AddDynamic(this, &ThisClass::StartSpawnPickupTimer);
		}
	}
}

void APickupSpawnPoint::SpawnPickupTimerFinished()
{
	if (HasAuthority()) {
		SpawnPickup();
	}
}

void APickupSpawnPoint::StartSpawnPickupTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnPickupTimeMin, SpawnPickupTimeMax);
	GetWorldTimerManager().SetTimer(
		SpawnPickupTimer,
		this,
		&ThisClass::SpawnPickupTimerFinished,
		SpawnTime
	);
}
