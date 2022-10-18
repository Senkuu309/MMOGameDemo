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
	//��������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	virtual void OnRep_PlayerState() override;


	//ǰ���ƶ�
	void MoveForward(float Value);

	//�����ƶ�
	void MoveRight(float Value);

	//���
	void SprintStart();

	void SprintStop();

	//�������
	virtual void MBLAttack();

	//�Ҽ�����
	virtual void MBRAttack();

	//���⼼��
	virtual void ExtraSkill();

	//����
	void PrimaryInteract();

	//��Ծ
	void Jump();

	bool ASCInputBound = false;

	FGameplayTag DeadTag;
};
