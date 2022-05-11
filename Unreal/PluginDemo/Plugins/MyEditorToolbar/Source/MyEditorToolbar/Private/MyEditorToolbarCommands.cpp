// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorToolbarCommands.h"

#define LOCTEXT_NAMESPACE "FMyEditorToolbarModule"

void FMyEditorToolbarCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MyEditorToolbar", "Execute MyEditorToolbar action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
