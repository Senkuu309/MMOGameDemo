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

UCLASS()
class MMOGAMEDEMO_API AXCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()


public:
	AXCharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual float GetHealth();

	UFUNCTION(BlueprintCallable)
	virtual float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bNewAttackStatus);

	UFUNCTION(BlueprintCallable)
	class AXWeaponActor* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	bool SetCurrentWeapon(AXWeaponActor* Weapon);

public:
	//相机臂组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//相机组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

protected:
	bool bIsAttacking;

	virtual void OnRep_PlayerState() override;

	//前后移动
	void MoveForward(float value);

	//左右移动
	void MoveRight(float value);

	//冲刺
	void SprintStart();

	void SprintStop();

	//左键攻击
	virtual void MBLAttack();

	//右键攻击
	virtual void MBRAttack();

	//额外技能
	virtual void ExtraSkill();

	//交互
	void PrimaryInteract();

	//跳跃
	void Jump();

	void GetWeaponList();

	bool ASCInputBound = false;

	FGameplayTag DeadTag;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	class AXWeaponActor* CurrentWeapon;
protected:
	//GAS组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UXAbilitySystemComponent> AbilitySystemComp;
	//属性组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UXAttributeSetBase> AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<class UXGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY()
	uint8 bAbilityInitialized:1;

	FGameplayTagContainer TagContainer;


protected:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddStartupGameplayAbilities();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthDamaged(float DamageValue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleHealthDamage(float DamageValue, const struct FGameplayTagContainer& EventTags);

	friend UXAttributeSetBase;
};
