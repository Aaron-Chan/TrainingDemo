// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMyEditorToolbarModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:
	void AddMenuBarExtension(FMenuBarBuilder& Builder);
	void AddToolBarExtension(FToolBarBuilder& Builder);
	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
