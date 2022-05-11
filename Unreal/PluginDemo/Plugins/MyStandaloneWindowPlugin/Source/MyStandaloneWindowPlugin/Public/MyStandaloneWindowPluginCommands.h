// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MyStandaloneWindowPluginStyle.h"

class FMyStandaloneWindowPluginCommands : public TCommands<FMyStandaloneWindowPluginCommands>
{
public:

	FMyStandaloneWindowPluginCommands()
		: TCommands<FMyStandaloneWindowPluginCommands>(TEXT("MyStandaloneWindowPlugin"), NSLOCTEXT("Contexts", "MyStandaloneWindowPlugin", "MyStandaloneWindowPlugin Plugin"), NAME_None, FMyStandaloneWindowPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};