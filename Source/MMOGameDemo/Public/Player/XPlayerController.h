// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "XPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:
	virtual void OnPossess(APawn* aPawn) override;

	virtual void OnRep_PlayerState() override;
};
