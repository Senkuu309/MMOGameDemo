// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill06.h"

UXGA_CharacterSkill06::UXGA_CharacterSkill06()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill06"));
	AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill05"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill06"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationRequiredTags.AddTag(AbilityNeedTag);

	AbilityInputID = EXAbilityInputID::RMB;
}
