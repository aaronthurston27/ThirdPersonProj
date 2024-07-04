// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonProjGameMode.h"
#include "ThirdPersonProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonProjGameMode::AThirdPersonProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
