// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameAbility/XAttributeSetBase.h"
#include "GameplayAbilitySpec.h"
#include "GameAbility/XGameplayAbility.h"

AXCharacterBase::AXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

class UAbilitySystemComponent* AXCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp.Get();
}

int32 AXCharacterBase::GetAbilityLevel(EXAbilityInputID AbilityID) const
{
	if (AttributeSetBase.IsValid())
	{
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}

	return 0;
}


void AXCharacterBase::AddCharacterAbilities()
{
	UE_LOG(LogTemp, Warning, TEXT("AXCharacterBase::AddCharacterAbilities"));
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComp.IsValid() || AbilitySystemComp->CharacterAbilitiesGiven)
	{
		return;
	}
	

	for (TSubclassOf<UXGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityId is %s"), *GetNameSafe(StartupAbility));
		AbilitySystemComp->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 
				GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), 
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), 
				this));
	}

	AbilitySystemComp->CharacterAbilitiesGiven = true;
}

