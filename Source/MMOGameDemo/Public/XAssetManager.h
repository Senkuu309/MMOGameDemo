// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "XAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	UXAssetManager();
	
	static UXAssetManager& Get();

	static const FPrimaryAssetType WeaponItemType;
	static const FPrimaryAssetType PotionItemType;

	virtual void StartInitialLoading() override;
	
	class UXItemBase* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
