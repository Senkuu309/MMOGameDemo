// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/XCharacterBase.h"
#include "XHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXHeroCharacter : public AXCharacterBase
{

	GENERATED_BODY()

public:
	//相机臂组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//相机组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	//武器组件
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* WeaponComp;

	//武器位置
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation1;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation2;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation3;

protected:
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

	void BindASCInput();

	bool ASCInputBound = false;

	FGameplayTag DeadTag;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	AXHeroCharacter();

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void OnRep_PlayerState() override;

};
