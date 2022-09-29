// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/XPlayerState.h"
#include "GameAbility/XAbilitySystemComponent.h"

AXPlayerState::AXPlayerState()
{
	AbilitySystemComp = CreateDefaultSubobject<UXAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
}

UAbilitySystemComponent* AXPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}
