// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/XANS_CanJumpNextCombo.h"
#include "AbilitySystemBlueprintLibrary.h"

void UXANS_CanJumpNextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	FGameplayEventData EventData;
	EventData.Instigator = MeshComp->GetOwner();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), FGameplayTag::RequestGameplayTag(TEXT("Event.Montage.CanJumpNextCombo")), EventData);
}

void UXANS_CanJumpNextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	FGameplayEventData EventData;
	EventData.Instigator = MeshComp->GetOwner();

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), FGameplayTag::RequestGameplayTag(TEXT("Event.Montage.CannotJumpNextCombo")), EventData);
}
