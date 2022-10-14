// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "XWeaponActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeginWeaponAttack, FGameplayTag, EventTag, float, AttackDelayTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndWeaponAttack, FGameplayTag, EventTag);

UCLASS()
class MMOGAMEDEMO_API AXWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AXWeaponActor();

	UPROPERTY()
	FOnBeginWeaponAttack OnBeginWeaponAttack;

	UPROPERTY()
	FOnEndWeaponAttack OnEndWeaponAttack;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class USphereComponent* RootComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UStaticMeshComponent* WeaponComp;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UCapsuleComponent* DefaultCollision;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class AXCharacterBase* WeaponUser;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void BeginWeaponAttack(FGameplayTag EventTag, float AttackDelayTime);

	UFUNCTION()
	void EndWeaponAttack(FGameplayTag EventTag);

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	bool bIsAttacking;

	TArray<AActor*> HitActors;

public:
	UFUNCTION(BlueprintCallable)
	bool GetIsAttacking();

	UFUNCTION(BlueprintCallable)
	void SetIsAttacking(bool bNewAttackStatus);

	UFUNCTION(BlueprintCallable)
	class AXCharacterBase* GetWeaponUser();

	UFUNCTION(BlueprintCallable)
	void SetWeaponUser(class AXCharacterBase* User);

	UFUNCTION(BlueprintCallable)
	bool AttachToCharacter();
};
