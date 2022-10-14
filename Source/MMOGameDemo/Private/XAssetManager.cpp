// Fill out your copyright notice in the Description page of Project Settings.


#include "XAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "DataAsset/XItemBase.h"

//const FPrimaryAssetType	UXAssetManager::PotionItemType = TEXT("Potion");
//const FPrimaryAssetType	UXAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	UXAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	UXAssetManager::WeaponItemType = TEXT("Weapon");

UXAssetManager::UXAssetManager()
{
}

UXAssetManager& UXAssetManager::Get()
{
	UXAssetManager* Singleton = Cast<UXAssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be GDAssetManager!"));
		return *NewObject<UXAssetManager>();	 // never calls this
	}
}

void UXAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}

UXItemBase* UXAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning /*= true*/)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	UXItemBase* LoadedItem = Cast<UXItemBase>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}
