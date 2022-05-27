// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReflectionSearchPluginCommands.h"

#define LOCTEXT_NAMESPACE "FReflectionSearchPluginModule"

void FReflectionSearchPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ReflectionSearchPlugin", "Bring up ReflectionSearchPlugin window", EUserInterfaceActionType::Button, FInputGesture());
	UI_COMMAND(OpenSearchWindow, "OpenActorWindow", "Bring up ReflectionSearchPlugin window for Actor", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
