// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hero/XCharacterHero.h"
#include "Character/XCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/XWeaponComponent.h"
#include "Character/XWeaponActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameAbility/XAttributeSetBase.h"
#include "GameplayAbilitySpec.h"
#include "GameAbility/XGameplayAbility.h"
#include "Player/XPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/PrimitiveComponent.h"
#include "XGameInstanceBase.h"
#include "XAssetManager.h"
#include "UObject/NoExportTypes.h"
#include "ActorFactories/ActorFactoryClass.h"
#include "Engine/World.h"

AXCharacterHero::AXCharacterHero()
{
	bAbilityInitialized = false;
	bIsAttacking = false;

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
}

void AXCharacterHero::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AXCharacterHero::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AXCharacterHero::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AXCharacterHero::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AXCharacterHero::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AXCharacterHero::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AXCharacterHero::SprintStop);

	PlayerInputComponent->BindAction("MBL", IE_Pressed, this, &AXCharacterHero::MBLAttack);
	PlayerInputComponent->BindAction("MBR", IE_Pressed, this, &AXCharacterHero::MBRAttack);
	PlayerInputComponent->BindAction("ExtraSkill", IE_Pressed, this, &AXCharacterHero::ExtraSkill);

	if (AbilitySystemComp && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EXAbilityInputID",
			static_cast<int32>(EXAbilityInputID::Confirm),
			static_cast<int32>(EXAbilityInputID::Cancel));

		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}



void AXCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	//if (PS)
	//{
	//	AbilitySystemComp = Cast<UXAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	//	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	//}

	if (AbilitySystemComp)
	{
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}

	AddStartupGameplayAbilities();
}

void AXCharacterHero::BeginPlay()
{
	Super::BeginPlay();
}

void AXCharacterHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AbilitySystemComp->GetOwnedGameplayTags(TagContainer);

	FString DebugMsg = GetNameSafe(this) + " : ";
	for (FGameplayTag Gameplaytag : TagContainer)
	{
		DebugMsg += Gameplaytag.ToString();
	}
	TagContainer.Reset();

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void AXCharacterHero::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AXCharacterHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//AXPlayerState* PS = GetPlayerState<AXPlayerState>();
	//if (PS)
	//{
	//	AbilitySystemComp = Cast<UXAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	//	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	//	AbilitySystemComp->SetTagMapCount(DeadTag, 0);
	//}

	AbilitySystemComp->InitAbilityActorInfo(this, this);

	if (AbilitySystemComp && InputComponent)
	{
		const FGameplayAbilityInputBinds Binds(
			"Confirm",
			"Cancel",
			"EXAbilityInputID",
			static_cast<int32>(EXAbilityInputID::Confirm),
			static_cast<int32>(EXAbilityInputID::Cancel));

		AbilitySystemComp->BindAbilityActivationToInputComponent(InputComponent, Binds);
	}
}


void AXCharacterHero::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AXCharacterHero::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AXCharacterHero::SprintStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Start!"));
}

void AXCharacterHero::SprintStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint Stop!"));
}

void AXCharacterHero::MBLAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBL Attack!"));
}

void AXCharacterHero::MBRAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("MBR Attack!"));
}

void AXCharacterHero::ExtraSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Extra Skill!"));
}

void AXCharacterHero::PrimaryInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Interact!"));
}

void AXCharacterHero::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump!"));
}
