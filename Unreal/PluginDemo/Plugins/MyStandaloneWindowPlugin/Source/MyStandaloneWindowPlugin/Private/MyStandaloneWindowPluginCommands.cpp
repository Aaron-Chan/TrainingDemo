// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyStandaloneWindowPluginCommands.h"

#define LOCTEXT_NAMESPACE "FMyStandaloneWindowPluginModule"

void FMyStandaloneWindowPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MyStandaloneWindowPlugin", "Bring up MyStandaloneWindowPlugin window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
