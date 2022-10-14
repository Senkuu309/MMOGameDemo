// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill02.h"

UXGA_CharacterSkill02::UXGA_CharacterSkill02()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill02"));
	AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill01"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill02"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationRequiredTags.AddTag(AbilityNeedTag);
}
