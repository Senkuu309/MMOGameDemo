// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill04.h"

UXGA_CharacterSkill04::UXGA_CharacterSkill04()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill04"));
	AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill03"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill04"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationRequiredTags.AddTag(AbilityNeedTag);

	AbilityInputID = EXAbilityInputID::LMB;
}
