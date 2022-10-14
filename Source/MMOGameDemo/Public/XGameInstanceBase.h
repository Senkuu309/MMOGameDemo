// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "../XObjectMacros.h"
#include "DataAsset/XItemBase.h"
#include "XGameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	void AutoLoadDataAssets();

	UPROPERTY(BlueprintReadOnly, Category = "Items")
	TArray<UXItemBase*> StoreItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FPrimaryAssetType> ItemSlotsPerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FPrimaryAssetId DefaultInventory;

	UFUNCTION(BlueprintCallable)
	TArray<UXItemBase*> GetStoreItems(FPrimaryAssetType ItemType);
	
};
