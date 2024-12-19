// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState->PlayerArray.Num();
	
	if (NumberOfPlayers == 2) {
		bUseSeamlessTravel = true;
		FString Level = LevelToLoad.LoadSynchronous()->GetFName().ToString() + "?listen";
		GetWorld()->ServerTravel(Level);
	}
}
