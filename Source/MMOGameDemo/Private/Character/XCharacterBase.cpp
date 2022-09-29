// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameAbility/XAttributeSetBase.h"
#include "GameplayAbilitySpec.h"
#include <GameAbility/XGameplayAbility.h>

AXCharacterBase::AXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//���������������󶨵���ɫ��
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;  //�������ת
	SpringArmComp->SetupAttachment(RootComponent);

	//�������������󶨵��������
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//�����������
	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>("WeaponComp");
	WeaponLocation1 = WeaponComp->GetSocketLocation("Start");
	WeaponLocation2 = WeaponComp->GetSocketLocation("Mid");
	WeaponLocation3 = WeaponComp->GetSocketLocation("End");
}

class UAbilitySystemComponent* AXCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp.Get();
}

int32 AXCharacterBase::GetAbilityLevel(EXAbilityInputID AbilityID) const
{
	if (AttributeSetBase.IsValid())
	{
		return static_cast<int32>(AttributeSetBase->GetCharacterLevel());
	}

	return 0;
}


void AXCharacterBase::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComp.IsValid() || AbilitySystemComp->CharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UXGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComp->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, 
				GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), 
				static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), 
				this));
	}

	AbilitySystemComp->CharacterAbilitiesGiven = true;
}

