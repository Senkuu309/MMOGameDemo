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
	//��������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USpringArmComponent* SpringArmComp;

	//������
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* CameraComp;

	//�������
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* WeaponComp;

	//GAS���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UXAbilitySystemComponent> AbilitySystemComp;

	//GA���
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GameplayAbility")
	TObjectPtr<UGameplayAbility> InGameplayAbility;

	//����λ��
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation1;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation2;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FVector WeaponLocation3;

protected:
	virtual void BeginPlay() override;

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

private:
	TMap<FName, FGameplayAbilitySpecHandle> Skills;

	

public:
	//ע������
	FGameplayAbilitySpecHandle RegisterGameAbility();

	//��������
	bool ActiveSkill(FName SkillName);

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
