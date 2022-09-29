// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

AXCharacterBase::AXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//创建相机臂组件，绑定到角色上
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;  //相机臂旋转
	SpringArmComp->SetupAttachment(RootComponent);

	//创建相机组件，绑定到相机臂上
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

	//创建武器组件
	WeaponComp = CreateDefaultSubobject<UStaticMeshComponent>("WeaponComp");
	WeaponLocation1 = WeaponComp->GetSocketLocation("Start");
	WeaponLocation2 = WeaponComp->GetSocketLocation("Mid");
	WeaponLocation3 = WeaponComp->GetSocketLocation("End");

	AbilitySystemComp = CreateDefaultSubobject<UXAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
}


FGameplayAbilitySpecHandle AXCharacterBase::RegisterGameAbility()
{
	if (AbilitySystemComp && IsValid(InGameplayAbility))
	{
		FGameplayAbilitySpecHandle Handle = AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(InGameplayAbility));
	
		return Handle;
	}

	return FGameplayAbilitySpecHandle();
}

bool AXCharacterBase::ActiveSkill(FName SkillName)
{
	if (AbilitySystemComp)
	{
		if (const FGameplayAbilitySpecHandle* Handle = Skills.Find(SkillName))
		{
			return AbilitySystemComp->TryActivateAbility(*Handle);
		}
	}
	return false;
}

void AXCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

void AXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AXCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AXCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AXCharacterBase::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AXCharacterBase::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AXCharacterBase::SprintStop);

	PlayerInputComponent->BindAction("MBL", IE_Pressed, this, &AXCharacterBase::MBLAttack);
	PlayerInputComponent->BindAction("MBR", IE_Pressed, this, &AXCharacterBase::MBRAttack);
	PlayerInputComponent->BindAction("ExtraSkill", IE_Pressed, this, &AXCharacterBase::ExtraSkill);

}

void AXCharacterBase::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AXCharacterBase::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AXCharacterBase::SprintStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Start!"));
}

void AXCharacterBase::SprintStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Stop!"));
}

void AXCharacterBase::MBLAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBL Attack!"));
}

void AXCharacterBase::MBRAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBR Attack!"));
}

void AXCharacterBase::ExtraSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Extra Skill!"));
}

void AXCharacterBase::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Interact!"));
}