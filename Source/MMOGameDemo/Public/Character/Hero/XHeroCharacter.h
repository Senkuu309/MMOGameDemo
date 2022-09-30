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
	//��������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	//������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	//�������
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* WeaponComp;

	//����λ��
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation1;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation2;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation3;

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
