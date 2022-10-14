// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/XItemBase.h"
#include "XPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXPotionItem : public UXItemBase
{
	GENERATED_BODY()

public:
	UXPotionItem()
	{
		ItemType = UXAssetManager::PotionItemType;
	}
	
};
