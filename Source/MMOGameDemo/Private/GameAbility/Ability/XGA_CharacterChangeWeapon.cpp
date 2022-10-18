// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterChangeWeapon.h"
#include "Character/XCharacterBase.h"
#include "Character/XWeaponActor.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "DataAsset/XWeaponItem.h"

UXGA_CharacterChangeWeapon::UXGA_CharacterChangeWeapon()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Combo")));
}

void UXGA_CharacterChangeWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		AXCharacterBase* Character = CastChecked<AXCharacterBase>(ActorInfo->AvatarActor.Get());
		
		if (Character->ChangeWeapon(NewWeaponItem, Character->GetCurrentWeaponItem()))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
		EndAbility(Handle, ActorInfo, ActivationInfo, true,true);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}