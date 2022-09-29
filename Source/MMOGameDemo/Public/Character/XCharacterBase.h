// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayAbilitySpec.h"
#include "XCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UXAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class MMOGAMEDEMO_API AXCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AXCharacterBase();

protected:
	//相机臂组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	//相机组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	//武器组件
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* WeaponComp;

	//GAS组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UXAbilitySystemComponent> AbilitySystemComp;

	//GA组件
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameplayAbility")
	TObjectPtr<UGameplayAbility> InGameplayAbility;

	//武器位置
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation1;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation2;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation3;

protected:
	virtual void BeginPlay() override;

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

private:
	TMap<FName, FGameplayAbilitySpecHandle> Skills;

	

public:
	//注册能力
	FGameplayAbilitySpecHandle RegisterGameAbility();

	//激活能力
	bool ActiveSkill(FName SkillName);

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
