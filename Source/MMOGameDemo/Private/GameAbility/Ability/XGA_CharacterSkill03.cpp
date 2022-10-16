// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill03.h"

UXGA_CharacterSkill03::UXGA_CharacterSkill03()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill03"));
	AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill02"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill03"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationRequiredTags.AddTag(AbilityNeedTag);

	AbilityInputID = EXAbilityInputID::LMB;
}
