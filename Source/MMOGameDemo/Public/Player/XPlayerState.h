// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "XPlayerState.generated.h"

class AXWeaponActor;
class UXWeaponItem;

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AXPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UXAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	bool IsAlive() const;

	AXWeaponActor* GetWeaponActor();

	void SetWeaponActor(AXWeaponActor* WeaponActor);

	UXWeaponItem* GetWeaponItem();

	void SetWeaponItem(UXWeaponItem* WeaponItem);


	//ªÒ»° Ù–‘
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;



protected:
	UPROPERTY()
	class UXAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY()
	class UXAttributeSetBase* AttributeSetBase;

	AXWeaponActor* CurrentWeapon;

	UXWeaponItem* CurrentWeaponItem;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);



	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
