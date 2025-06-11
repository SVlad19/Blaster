// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/BlasterPlayerState.h"
#include "Character/BlasterCharacter.h"
#include "PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}

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
	SetScore(Score + ScoreAmount);
	Character = Character.IsValid() ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character.IsValid()) {
		Controller = Controller.IsValid() ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller.IsValid()) {
			Controller->SetHUDScore(Score);
		}
	}
}

void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;
	Character = Character.IsValid() ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character.IsValid()) {
		Controller = Controller.IsValid() ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller.IsValid()) {
			Controller->SetHUDDefeats(Defeats);
		}
	}
}

void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character.IsValid() ? Character : Cast<ABlasterCharacter>(GetPawn());
	if (Character.IsValid()) {
		Controller = Controller.IsValid() ? Controller : Cast<ABlasterPlayerController>(Character->GetController());
		if (Controller.IsValid()) {
			Controller->SetHUDDefeats(Defeats);
		}
	}
}
