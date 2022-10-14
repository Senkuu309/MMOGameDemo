// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "XWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXWeaponComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UXWeaponComponent();

	virtual void BeginPlay() override;

	TArray<AActor*> HitActors;

protected:
	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void WeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
