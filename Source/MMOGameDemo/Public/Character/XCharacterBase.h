// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "../XObjectMacros.h"
#include "GameplayEffect.h"
#include "DataAsset/XWeaponItem.h"
#include "XCharacterBase.generated.h"

class UXWeaponItem;
class AXWeaponActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AXCharacterBase*, Character);

UCLASS()
class MMOGAMEDEMO_API AXCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	AXCharacterBase();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "GASDocumentation|GDCharacter")
	FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bNewAttackStatus);

	UFUNCTION(BlueprintCallable)
	AXWeaponActor* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	UXWeaponItem* GetCurrentWeaponItem();

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	bool ChangeWeapon(UXWeaponItem* NewWeapon, UXWeaponItem* OldWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void ChangeWeaponActor(UXWeaponItem* NewWeapon, UXWeaponItem* OldWeapon);

	void AddStartupGameplayAbilities();

	virtual void RemoveCharacterAbilities();

	virtual void HandleHealthDamage(float DamageValue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintCallable)
	void SetAbilitySystemComponent(UXAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	virtual void FinishDying();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AXWeaponActor* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UXWeaponItem* CurrentWeaponItem;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	TArray<class UXWeaponItem*> WeaponList;

	//GAS组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UXAbilitySystemComponent> AbilitySystemComp;
	//属性组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UXAttributeSetBase> AttributeSetBase;

	bool bIsAttacking;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UXGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY()
	uint8 bAbilityInitialized:1;

	FGameplayTagContainer TagContainer;
	
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	UAnimMontage* DeathMontage;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthDamaged(float DamageValue, const struct FGameplayTagContainer& EventTags);

	virtual void SetHealth(float Health);
};
