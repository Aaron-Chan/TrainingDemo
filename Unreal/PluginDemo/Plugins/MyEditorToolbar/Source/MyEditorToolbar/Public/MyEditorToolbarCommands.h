// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MyEditorToolbarStyle.h"

class FMyEditorToolbarCommands : public TCommands<FMyEditorToolbarCommands>
{
public:

	FMyEditorToolbarCommands()
		: TCommands<FMyEditorToolbarCommands>(TEXT("MyEditorToolbar"), NSLOCTEXT("Contexts", "MyEditorToolbar", "MyEditorToolbar Plugin"), NAME_None, FMyEditorToolbarStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
