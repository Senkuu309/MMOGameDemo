// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill07.h"

UXGA_CharacterSkill07::UXGA_CharacterSkill07()
{
	AbilityTag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Skill07"));
	AbilityNeedTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill06"));
	AbilityProvideTag = FGameplayTag::RequestGameplayTag(FName("Combo.Skill07"));

	AbilityTags.AddTag(AbilityTag);
	ActivationOwnedTags.AddTag(AbilityTag);
	ActivationRequiredTags.AddTag(AbilityNeedTag);

	AbilityInputID = EXAbilityInputID::RMB;
}
