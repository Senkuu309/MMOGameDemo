// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/XPlayerController.h"
#include "Player/XPlayerState.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "Player/XPlayerState.h"
#include "XGameInstanceBase.h"
#include "XAssetManager.h"


void AXPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	return;
}

bool AXPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

void AXPlayerController::BeginPlay()
{

}

bool AXPlayerController::LoadInventory()
{
	UWorld* World = GetWorld();
	UXGameInstanceBase* GameInstance = World ? World->GetGameInstance<UXGameInstanceBase>() : nullptr;

	if(!GameInstance)
	{
		return false;
	}

	UXAssetManager& AssetManager = UXAssetManager::Get();

	CurWeaponItem = AssetManager.ForceLoadItem(GameInstance->DefaultInventory);

	return true;
}

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
