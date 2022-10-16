// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill08.h"

UXGA_CharacterSkill08::UXGA_CharacterSkill08()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill08"));
	AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill07"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill08"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationRequiredTags.AddTag(AbilityNeedTag);

	AbilityInputID = EXAbilityInputID::RMB;
}
