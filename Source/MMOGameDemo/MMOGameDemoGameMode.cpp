// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOGameDemoGameMode.h"
#include "MMOGameDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMMOGameDemoGameMode::AMMOGameDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
