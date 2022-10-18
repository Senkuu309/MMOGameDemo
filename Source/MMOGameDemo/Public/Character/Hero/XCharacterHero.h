// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/XCharacterBase.h"
#include "GameplayTagContainer.h"
#include "XCharacterHero.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXCharacterHero : public AXCharacterBase
{
	GENERATED_BODY()

public:
	AXCharacterHero();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

public:
	//相机臂组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//相机组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void OnRep_PlayerState() override;


	//前后移动
	void MoveForward(float Value);

	//左右移动
	void MoveRight(float Value);

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

	bool ASCInputBound = false;

	FGameplayTag DeadTag;
};
