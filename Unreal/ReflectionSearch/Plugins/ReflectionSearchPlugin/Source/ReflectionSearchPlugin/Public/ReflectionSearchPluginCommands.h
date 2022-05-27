// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ReflectionSearchPluginStyle.h"

class FReflectionSearchPluginCommands : public TCommands<FReflectionSearchPluginCommands>
{
public:

	FReflectionSearchPluginCommands()
		: TCommands<FReflectionSearchPluginCommands>(TEXT("ReflectionSearchPlugin"), NSLOCTEXT("Contexts", "ReflectionSearchPlugin", "ReflectionSearchPlugin Plugin"), NAME_None, FReflectionSearchPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
	TSharedPtr< FUICommandInfo > OpenSearchWindow;
};