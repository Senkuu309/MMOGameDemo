// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/XHeroCharacter.h"
#include "Player/XPlayerState.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "../XObjectMacros.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AXHeroCharacter::AXHeroCharacter()
{
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
}

void AXHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AXHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AXHeroCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AXHeroCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AXHeroCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AXHeroCharacter::SprintStop);

	PlayerInputComponent->BindAction("MBL", IE_Pressed, this, &AXHeroCharacter::MBLAttack);
	PlayerInputComponent->BindAction("MBR", IE_Pressed, this, &AXHeroCharacter::MBRAttack);
	PlayerInputComponent->BindAction("ExtraSkill", IE_Pressed, this, &AXHeroCharacter::ExtraSkill);

	BindASCInput();
}

void AXHeroCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AXHeroCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AXHeroCharacter::SprintStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Start!"));
}

void AXHeroCharacter::SprintStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Stop!"));
}

void AXHeroCharacter::MBLAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBL Attack!"));
}

void AXHeroCharacter::MBRAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBR Attack!"));
}

void AXHeroCharacter::ExtraSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Extra Skill!"));
}

void AXHeroCharacter::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Interact!"));
}

void AXHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	UE_LOG(LogTemp, Warning, TEXT("Server playerState is %s"), *GetNameSafe(PS));
	if (PS)
	{
		AbilitySystemComp = Cast<UXAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
	
	AddCharacterAbilities();
}

void AXHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	UE_LOG(LogTemp, Warning, TEXT("Client playerState is %s"), *GetNameSafe(PS));
	if (PS)
	{
		AbilitySystemComp = Cast<UXAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		BindASCInput();

		AbilitySystemComp->SetTagMapCount(DeadTag, 0);
	}
}

void AXHeroCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComp.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("EXbilityInputID"), static_cast<int32>(EXAbilityInputID::Confirm), static_cast<int32>(EXAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}
