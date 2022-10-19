// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/XCharacterBase.h"
#include "Character/XWeaponActor.h"
#include "GameAbility/XAttributeSetBase.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameAbility/XGameplayAbility.h"
#include "Player/XPlayerState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


AXCharacterBase::AXCharacterBase()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	PrimaryActorTick.bCanEverTick = false;
	
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
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

UXWeaponItem* AXCharacterBase::GetCurrentWeaponItem()
{
	return CurrentWeaponItem;
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
		AXPlayerState* PS = Cast<AXPlayerState>(GetPlayerState());
		UE_LOG(LogTemp, Warning, TEXT("Server CurrentWeapon is %s"), *GetNameSafe(PS->GetWeaponActor()));
	}

	return true;
}

void AXCharacterBase::ChangeWeaponActor_Implementation(UXWeaponItem* NewWeapon, UXWeaponItem* OldWeapon)
{
	AXPlayerState* PS = Cast<AXPlayerState>(GetPlayerState());
	if (PS)
	{
		//修改动画蓝图
		GetMesh()->SetAnimInstanceClass(NewWeapon->AnimBlurprint);

		//修改武器外观
		CurrentWeapon->Destroy(true);
		CurrentWeapon = nullptr;

		PS->SetWeaponItem(NewWeapon);
		CurrentWeaponItem = PS->GetWeaponItem();

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;

		FTransform DefautTransform = GetTransform();

		PS->SetWeaponActor(Cast<AXWeaponActor>(GetWorld()->SpawnActor(CurrentWeaponItem->WeaponActor, &DefautTransform, SpawnParameters)));
		CurrentWeapon = PS->GetWeaponActor();

		CurrentWeapon->SetWeaponUser(this);
		CurrentWeapon->AttachToCharacter();
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
		AbilitySystemComp->CharacterAbilitiesGiven = true;

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

void AXCharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComp || !AbilitySystemComp->CharacterAbilitiesGiven)
	{
		return;
	}

	TArray<FGameplayAbilitySpecHandle> AbilitiesToMove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComp->GetActivatableAbilities())
	{
		AbilitiesToMove.Add(Spec.Handle);
	}

	for (int32 i = 0; i < AbilitiesToMove.Num(); ++i)
	{
		AbilitySystemComp->ClearAbility(AbilitiesToMove[i]);
	}
	AbilitySystemComp->CharacterAbilitiesGiven = false;
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

bool AXCharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AXCharacterBase::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComp)
	{
		AbilitySystemComp->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComp->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComp->AddLooseGameplayTag(DeadTag);
	}
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}

}

void AXCharacterBase::FinishDying_Implementation()
{
	CurrentWeapon->Destroy();
	Destroy();
}

void AXCharacterBase::SetHealth(float Health)
{
	if (AttributeSetBase)
	{
		AttributeSetBase->SetHealth(Health);
	}
}