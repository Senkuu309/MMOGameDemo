// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "XPlayerState.generated.h"

class UAbilitySystemComponent;
class UXAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	UXAbilitySystemComponent* AbilitySystemComp;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

};
