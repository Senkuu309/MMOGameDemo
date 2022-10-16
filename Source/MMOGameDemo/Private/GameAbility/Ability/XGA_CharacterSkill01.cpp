// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill01.h"

UXGA_CharacterSkill01::UXGA_CharacterSkill01()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill01"));
	//AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName(""));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill01"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combo")));

	AbilityInputID = EXAbilityInputID::LMB;
}
