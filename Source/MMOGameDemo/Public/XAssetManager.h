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
	static UXAssetManager& Get();

	virtual void StartInitialLoading() override;
	
};
