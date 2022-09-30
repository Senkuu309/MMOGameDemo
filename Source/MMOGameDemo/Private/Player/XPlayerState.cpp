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

void AXPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComp->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}