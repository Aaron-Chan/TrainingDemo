// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModePlugin.h"
#include "MyEditorModePluginEdMode.h"

#define LOCTEXT_NAMESPACE "FMyEditorModePluginModule"

void FMyEditorModePluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FMyEditorModePluginEdMode>(FMyEditorModePluginEdMode::EM_MyEditorModePluginEdModeId, LOCTEXT("MyEditorModePluginEdModeName", "MyEditorModePluginEdMode"), FSlateIcon(), true);
}

void FMyEditorModePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FMyEditorModePluginEdMode::EM_MyEditorModePluginEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyEditorModePluginModule, MyEditorModePlugin)