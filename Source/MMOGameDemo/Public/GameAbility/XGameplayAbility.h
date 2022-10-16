// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include <../XObjectMacros.h>
#include "XGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UXGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Ability")
	EXAbilityInputID AbilityInputID { EXAbilityInputID::None};
};
