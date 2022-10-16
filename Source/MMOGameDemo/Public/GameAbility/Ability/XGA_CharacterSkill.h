// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../XGameplayAbility.h"
#include "XGA_CharacterSkill.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXGA_CharacterSkill : public UXGameplayAbility
{
	GENERATED_BODY()

public:
	UXGA_CharacterSkill();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skill")
	UAnimMontage* SkillMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skill")
	TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Skill")
	FGameplayTag AbilityTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Skill")
	FGameplayTag AbilityNeedTag;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Skill")
	FGameplayTag AbilityProvideTag;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void RemoveAbilityNeedTag();

protected:
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

};
