// Fill out your copyright notice in the Description page of Project Settings.


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

AXCharacterBase::AXCharacterBase()
{
	bReplicates = true;

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

	//创建GAS组件
	AbilitySystemComp = CreateDefaultSubobject<UXAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSetBase = CreateDefaultSubobject<UXAttributeSetBase>("AttributeSetBase");

}

void AXCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AXCharacterBase, CurrentWeaponItem);
	DOREPLIFETIME(AXCharacterBase, CurrentWeapon);

}

void AXCharacterBase::Tick(float DeltaTime)
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

void AXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

float AXCharacterBase::GetHealth()
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetHealth();
	}
	return -1.f;
}

float AXCharacterBase::GetMaxHealth()
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return -1.f;
}

bool AXCharacterBase::GetIsAttacking()
{
	return bIsAttacking;
}

void AXCharacterBase::SetIsAttacking(bool bNewAttackStatus)
{
	bIsAttacking = bNewAttackStatus;
}

AXWeaponActor* AXCharacterBase::GetCurrentWeapon()
{
	return CurrentWeapon;
}

bool AXCharacterBase::SetCurrentWeapon(AXWeaponActor* Weapon)
{
	if (Weapon)
	{
		CurrentWeapon = Weapon;
		return true;
	}
	return false;
}

UXWeaponItem* AXCharacterBase::GetCurrentWeaponItem()
{
	return CurrentWeaponItem;
}

bool AXCharacterBase::SetCurrentWeaponItem(UXWeaponItem* Weapon)
{
	if (Weapon)
	{
		CurrentWeaponItem = Weapon;
		return true;
	}
	return false;
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

void AXCharacterBase::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump!"));
}

bool AXCharacterBase::ChangeWeapon(UXWeaponItem* NewWeapon, UXWeaponItem* OldWeapon)
{
	if (!NewWeapon || !OldWeapon || !AbilitySystemComp || NewWeapon == OldWeapon)
	{
		return false;
	}
	if (GetLocalRole() == ROLE_Authority)
	{
		//移除旧武器的技能
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComp->GetActivatableAbilities())
		{
			if (OldWeapon->GrantedAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
		{
			AbilitySystemComp->ClearAbility(AbilitiesToRemove[i]);
		}

		//添加新武器技能
		for (auto NewWeaponAbility : NewWeapon->GrantedAbilities)
		{
			AbilitySystemComp->GiveAbility(
				FGameplayAbilitySpec(NewWeaponAbility, 1, static_cast<int32>(NewWeaponAbility.GetDefaultObject()->AbilityInputID), this));
		}
		ChangeWeaponActor(NewWeapon, OldWeapon);
	}

	return true;
}

void AXCharacterBase::ChangeWeaponActor_Implementation(UXWeaponItem* NewWeapon, UXWeaponItem* OldWeapon)
{
	//修改动画蓝图
	GetMesh()->SetAnimInstanceClass(NewWeapon->AnimBlurprint);

	//修改武器外观
	CurrentWeapon->Destroy(true);
	CurrentWeapon = nullptr;

	CurrentWeaponItem = NewWeapon;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;

	FTransform DefautTransform = GetTransform();

	CurrentWeapon = Cast<AXWeaponActor>(GetWorld()->SpawnActor(CurrentWeaponItem->WeaponActor, &DefautTransform, SpawnParameters));

	CurrentWeapon->SetWeaponUser(this);
	CurrentWeapon->AttachToCharacter();
}

void AXCharacterBase::PossessedBy(AController* NewController)
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

void AXCharacterBase::OnRep_PlayerState()
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

void AXCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AXCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AXCharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AXCharacterBase::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AXCharacterBase::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AXCharacterBase::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AXCharacterBase::SprintStop);

	PlayerInputComponent->BindAction("MBL", IE_Pressed, this, &AXCharacterBase::MBLAttack);
	PlayerInputComponent->BindAction("MBR", IE_Pressed, this, &AXCharacterBase::MBRAttack);
	PlayerInputComponent->BindAction("ExtraSkill", IE_Pressed, this, &AXCharacterBase::ExtraSkill);

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

class UAbilitySystemComponent* AXCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void AXCharacterBase::AddStartupGameplayAbilities()
{
	check(AbilitySystemComp);
	
	if (GetLocalRole() == ROLE_Authority && !bAbilityInitialized)
	{
		for (TSubclassOf<UXGameplayAbility>& StartupAbility : CharacterAbilities)
		{
			UE_LOG(LogTemp, Warning, TEXT("AbilityId is %s"), *GetNameSafe(StartupAbility));
			AbilitySystemComp->GiveAbility(
				FGameplayAbilitySpec(StartupAbility, 1, static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
		}

		for (const TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();

			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComp->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);

			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComp->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComp);
			}
		}
		bAbilityInitialized = true;
	}
}

void AXCharacterBase::HandleHealthDamage(float DamageValue, const struct FGameplayTagContainer& EventTags)
{
	if(bAbilityInitialized)
	{
		OnHealthDamaged(DamageValue, EventTags);
	}
}