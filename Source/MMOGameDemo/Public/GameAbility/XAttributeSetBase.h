// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "XAttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	UXAttributeSetBase();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Attribtues")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UXAttributeSetBase, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Attribtues")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UXAttributeSetBase, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UXAttributeSetBase, Damage)

	void AdjustAttributeForMaxChange(
		const FGameplayAttributeData& AffectedAttribute, 
		const FGameplayAttributeData& MaxAttribute, 
		float NewMaxValue, 
		const FGameplayAttribute& AffectedAttributeProperty) const;

protected:

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData OldValue);
	
};
