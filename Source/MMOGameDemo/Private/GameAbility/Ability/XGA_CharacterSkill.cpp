// Fill out your copyright notice in the Description page of Project Settings.


#include "GameAbility/Ability/XGA_CharacterSkill.h"
#include "AbilitySystemComponent.h"
#include "GameAbility/Task/XAbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbility.h"
#include "Character/XCharacterBase.h"
#include "GameAbility/XAbilitySystemComponent.h"

UXGA_CharacterSkill::UXGA_CharacterSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Skill"));

}

void UXGA_CharacterSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	RemoveAbilityNeedTag();

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UAnimMontage* MontageToPlay = SkillMontage;
	UE_LOG(LogTemp, Warning, TEXT("SkillMontage is %s"), *GetNameSafe(MontageToPlay));

	UXAbilityTask_PlayMontageAndWait* Task = UXAbilityTask_PlayMontageAndWait::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
	Task->OnBlendOut.AddDynamic(this, &UXGA_CharacterSkill::OnCompleted);
	Task->OnCompleted.AddDynamic(this, &UXGA_CharacterSkill::OnCompleted);
	Task->OnInterrupted.AddDynamic(this, &UXGA_CharacterSkill::OnCancelled);
	Task->OnCancelled.AddDynamic(this, &UXGA_CharacterSkill::OnCancelled);
	Task->EventReceived.AddDynamic(this, &UXGA_CharacterSkill::EventReceived);

	Task->ReadyForActivation();
}

void UXGA_CharacterSkill::RemoveAbilityNeedTag()
{
	UXAbilitySystemComponent* AbilitySystemComp = Cast<UXAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	if (AbilitySystemComp)
	{
		AbilitySystemComp->RemoveLooseGameplayTag(AbilityNeedTag);
	}
}

void UXGA_CharacterSkill::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("The Skill is Cancelled!"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UXGA_CharacterSkill::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("The Skill is Completed!"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UXGA_CharacterSkill::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("EventReceived!"));
	if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.EndAbility")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability End!"))
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	else if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.Hit")))
	{

		UE_LOG(LogTemp, Warning, TEXT("The hit actor is %s!"), *GetNameSafe(EventData.Target));

		FGameplayAbilityTargetData_ActorArray* NewTargetData_ActorArray = new FGameplayAbilityTargetData_ActorArray();
		NewTargetData_ActorArray->TargetActorArray.Add(const_cast<AActor*>(EventData.Target));

		FGameplayAbilityTargetDataHandle TargetHandleData;
		TargetHandleData.Add(NewTargetData_ActorArray);

		ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetHandleData, DamageGameplayEffect, 1, 1);
	}
	else if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.CanJumpNextCombo")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Event.Montage.CanJumpNextCombo"));

		UXAbilitySystemComponent* AbilitySystemComp = Cast<UXAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		if (AbilitySystemComp)
		{
			AbilitySystemComp->RemoveLooseGameplayTag(AbilityTag);
			AbilitySystemComp->AddLooseGameplayTag(AbilityProvideTag);
		}

	}
	else if (EventTag == FGameplayTag::RequestGameplayTag(FName("Event.Montage.CannotJumpNextCombo")))
	{
		UXAbilitySystemComponent* AbilitySystemComp = Cast<UXAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
		if (AbilitySystemComp)
		{
			AbilitySystemComp->RemoveLooseGameplayTag(AbilityProvideTag);
		}
	}
}
