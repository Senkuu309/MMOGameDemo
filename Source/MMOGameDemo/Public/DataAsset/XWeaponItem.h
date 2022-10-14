// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/XItemBase.h"
#include "XAssetManager.h"
#include "Character/XWeaponActor.h"
#include "XWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXWeaponItem : public UXItemBase
{
	GENERATED_BODY()
	
public:
	UXWeaponItem()
	{
		ItemType = UXAssetManager::WeaponItemType;
	};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AXWeaponActor> WeaponActor;
};
