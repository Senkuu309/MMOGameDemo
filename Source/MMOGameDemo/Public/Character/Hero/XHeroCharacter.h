// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/XCharacterBase.h"
#include "XHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API AXHeroCharacter : public AXCharacterBase
{
	GENERATED_BODY()

protected:
	//Ç°ºóÒÆ¶¯
	void MoveForward(float value);

	//×óÓÒÒÆ¶¯
	void MoveRight(float value);

	//³å´Ì
	void SprintStart();

	void SprintStop();

	//×ó¼ü¹¥»÷
	virtual void MBLAttack();

	//ÓÒ¼ü¹¥»÷
	virtual void MBRAttack();

	//¶îÍâ¼¼ÄÜ
	virtual void ExtraSkill();

	//½»»¥
	void PrimaryInteract();

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	AXHeroCharacter(const class FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

protected:
	virtual void OnRep_PlayerState() override;
};
