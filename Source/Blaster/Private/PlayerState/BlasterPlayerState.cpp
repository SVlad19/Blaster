// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"
#include "Character/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character.IsValid() ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character.IsValid()) {
		Controller = Controller.IsValid() ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller.IsValid()) {
			Controller->SetHUDScore(Score);
		}
	}
}

void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	Score += ScoreAmount;
	Character = Character.IsValid() ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character.IsValid()) {
		Controller = Controller.IsValid() ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller.IsValid()) {
			Controller->SetHUDScore(Score);
		}
	}
}
