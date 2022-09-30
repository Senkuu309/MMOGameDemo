// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "../XObjectMacros.h"
#include "XCharacterBase.generated.h"


UCLASS()
class MMOGAMEDEMO_API AXCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXCharacterBase();
protected:
	//GAS组件
	TWeakObjectPtr<class UXAbilitySystemComponent> AbilitySystemComp;
	//属性组件
	TWeakObjectPtr<class UXAttributeSetBase> AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GameAbility|Abilities")
	TArray<TSubclassOf<class UXGameplayAbility>> CharacterAbilities;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	//TSubclassOf<class UGameplayEffect> DefaultAttributes;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	//TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacterAbilities();



public:
	UFUNCTION(BlueprintCallable, Category = "GameAbility|XCharacter")
	virtual int32 GetAbilityLevel(EXAbilityInputID AbilityID) const;

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
