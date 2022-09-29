// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/XPlayerController.h"
#include "Player/XPlayerState.h"
#include "GameAbility/XAbilitySystemComponent.h"

void AXPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	if (PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, aPawn);
	}
}

void AXPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();


}
