// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/XPlayerState.h"
#include "GameAbility/XAbilitySystemComponent.h"
#include "GameAbility/XAttributeSetBase.h"
#include "Character/Hero/XCharacterHero.h"

AXPlayerState::AXPlayerState()
{
	AbilitySystemComp = CreateDefaultSubobject<UXAbilitySystemComponent>("AbilitySystemComp");
	AbilitySystemComp->SetIsReplicated(true);

	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	AttributeSetBase = CreateDefaultSubobject<UXAttributeSetBase>("AttributeSetBase");

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

void AXPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComp)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = AbilitySystemComp->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AXPlayerState::HealthChanged);
		//MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &AGDPlayerState::MaxHealthChanged);
		//HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute()).AddUObject(this, &AGDPlayerState::HealthRegenRateChanged);
		//ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute()).AddUObject(this, &AGDPlayerState::ManaChanged);
		//MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxManaAttribute()).AddUObject(this, &AGDPlayerState::MaxManaChanged);
		//ManaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaRegenRateAttribute()).AddUObject(this, &AGDPlayerState::ManaRegenRateChanged);
		//StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &AGDPlayerState::StaminaChanged);
		//MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &AGDPlayerState::MaxStaminaChanged);
		//StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute()).AddUObject(this, &AGDPlayerState::StaminaRegenRateChanged);
		//XPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetXPAttribute()).AddUObject(this, &AGDPlayerState::XPChanged);
		//GoldChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetGoldAttribute()).AddUObject(this, &AGDPlayerState::GoldChanged);
		//CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute()).AddUObject(this, &AGDPlayerState::CharacterLevelChanged);

		// Tag change callbacks
		AbilitySystemComp->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AXPlayerState::StunTagChanged);
	}
}


void AXPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	AXCharacterHero* Hero = Cast<AXCharacterHero>(GetPawn());
	if (Hero)
	{

	}

	if (!IsAlive() && !AbilitySystemComp->HasMatchingGameplayTag(DeadTag))
	{
		if (Hero)
		{
			Hero->Die();
		}
	}
}

UAbilitySystemComponent* AXPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

class UXAttributeSetBase* AXPlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool AXPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

AXWeaponActor* AXPlayerState::GetWeaponActor()
{
	return CurrentWeapon;
}

void AXPlayerState::SetWeaponActor(AXWeaponActor* WeaponActor)
{
	CurrentWeapon = WeaponActor;
}

UXWeaponItem* AXPlayerState::GetWeaponItem()
{
	return CurrentWeaponItem;
}

void AXPlayerState::SetWeaponItem(UXWeaponItem* WeaponItem)
{
	CurrentWeaponItem = WeaponItem;
}

float AXPlayerState::GetHealth() const
{
	return AttributeSetBase->GetHealth();
}

void AXPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComp->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}