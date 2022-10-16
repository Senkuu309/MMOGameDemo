// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill05.h"

UXGA_CharacterSkill05::UXGA_CharacterSkill05()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill05"));
	//AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill01"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill05"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	//ActivationRequiredTags.AddTag(AbilityNeedTag);
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combo")));

	AbilityInputID = EXAbilityInputID::RMB;
}
