// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/XANS_Attack.h"
#include "Character/XCharacterBase.h"
#include "Character/XWeaponActor.h"

void UXANS_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AXCharacterBase* Player = Cast<AXCharacterBase>(MeshComp->GetOwner());

	if (Player)
	{
		AXWeaponActor* CurrentWeapon = Player->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->OnBeginWeaponAttack.Broadcast(FGameplayTag::RequestGameplayTag(FName("Event.Weapon.Attack")), 0.04f);
			Player->SetIsAttacking(true);
		}
	}
}

void UXANS_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AXCharacterBase* Player = Cast<AXCharacterBase>(MeshComp->GetOwner());
	if (Player)
	{
		AXWeaponActor* CurrentWeapon = Player->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			CurrentWeapon->OnEndWeaponAttack.Broadcast(FGameplayTag::RequestGameplayTag(FName("Event.Weapon.Attack")));
			Player->SetIsAttacking(false);
		}
	}
}
