// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "XAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UXAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	UPROPERTY()
	FReceivedDamageDelegate ReceivedDamage;

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UXAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
