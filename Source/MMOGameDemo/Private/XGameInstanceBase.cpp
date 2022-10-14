// Fill out your copyright notice in the Description page of Project Settings.


#include "XGameInstanceBase.h"
#include "XAssetManager.h"
#include "DataAsset/XItemBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UXGameInstanceBase::Init()
{
	Super::Init();
}

void UXGameInstanceBase::AutoLoadDataAssets()
{
	UXAssetManager& Manager = UXAssetManager::Get();
	for (auto CurrentType : ItemSlotsPerType)
	{
		TArray<FPrimaryAssetId> AssetIds;
		if (Manager.GetPrimaryAssetIdList(CurrentType, AssetIds))
		{
		}
	}
}

TArray<UXItemBase*> UXGameInstanceBase::GetStoreItems(FPrimaryAssetType ItemType)
{
	TArray<UXItemBase*> FilterItems;
	for (UXItemBase* DataAsset : StoreItems)
	{
		if (DataAsset->GetPrimaryAssetId().PrimaryAssetType == ItemType)
		{
			FilterItems.Add(DataAsset);
		}
	}
	return FilterItems;
}
