// Fill out your copyright notice in the Description page of Project Settings.


#include "XAssetManager.h"
#include "AbilitySystemGlobals.h"

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
