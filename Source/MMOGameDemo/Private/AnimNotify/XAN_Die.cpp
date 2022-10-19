// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/XAN_Die.h"
#include "Character/XCharacterBase.h"

void UXAN_Die::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AXCharacterBase* Player = Cast<AXCharacterBase>(MeshComp->GetOwner());
	if (Player)
	{
		Player->FinishDying();
	}
}
