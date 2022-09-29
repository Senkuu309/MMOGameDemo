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

protected:
	//ǰ���ƶ�
	void MoveForward(float value);

	//�����ƶ�
	void MoveRight(float value);

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

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	AXHeroCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void OnRep_PlayerState() override;
};
