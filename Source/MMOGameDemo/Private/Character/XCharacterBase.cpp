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
}

void AXCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AXCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

float AXCharacterBase::GetHealth() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetHealth();
	}
	return -1.f;
}

float AXCharacterBase::GetMaxHealth() const
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
	if (bAbilityInitialized)
	{
		OnHealthDamaged(DamageValue, EventTags);
	}
}

void AXCharacterBase::SetAbilitySystemComponent(UXAbilitySystemComponent* AbilitySystemComponent)
{
	AbilitySystemComp = AbilitySystemComponent;
}

void AXCharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetHealth(Health);
	}
}