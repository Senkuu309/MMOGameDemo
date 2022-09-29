// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "XAttributeSetBase.generated.h"

/**
 * 
 */
UCLASS()
class MMOGAMEDEMO_API UXAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

public:
	int32 GetCharacterLevel();
	
};
