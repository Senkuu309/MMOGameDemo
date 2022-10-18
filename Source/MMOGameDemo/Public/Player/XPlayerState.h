// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "XPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UXAttributeSetBase* GetAttributeSetBase() const;

protected:
	UPROPERTY()
	class UXAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY()
	class UXAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	virtual void BeginPlay() override;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
