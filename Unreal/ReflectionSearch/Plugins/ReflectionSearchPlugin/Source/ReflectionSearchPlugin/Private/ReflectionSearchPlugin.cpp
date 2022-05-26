// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReflectionSearchPlugin.h"
#include "ReflectionSearchPluginStyle.h"
#include "ReflectionSearchPluginCommands.h"
#include "ReflectionSearchTool.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName ReflectionSearchPluginTabName("ReflectionSearchPlugin");

#define LOCTEXT_NAMESPACE "FReflectionSearchPluginModule"

void FReflectionSearchPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FReflectionSearchPluginStyle::Initialize();
	FReflectionSearchPluginStyle::ReloadTextures();

	FReflectionSearchPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FReflectionSearchPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FReflectionSearchPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FReflectionSearchPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ReflectionSearchPluginTabName, FOnSpawnTab::CreateRaw(this, &FReflectionSearchPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FReflectionSearchPluginTabTitle", "ReflectionSearchPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FReflectionSearchPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FReflectionSearchPluginStyle::Shutdown();

	FReflectionSearchPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ReflectionSearchPluginTabName);
}

TSharedRef<SDockTab> FReflectionSearchPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FReflectionSearchPluginModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("ReflectionSearchPlugin.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(ReflectionSearchTool)
			
		];
}

// void FReflectionSearchPluginModule::OnSearchTextCommited(const FText& InText, ETextCommit::Type InCommitType)
// {
// 	// TryRefreshingSearch(InText);
// }
//
// void FReflectionSearchPluginModule::OnSearchChanged(const FText& InFilterText)
// {
// 	
// }


void FReflectionSearchPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(ReflectionSearchPluginTabName);
}

void FReflectionSearchPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FReflectionSearchPluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FReflectionSearchPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FReflectionSearchPluginModule, ReflectionSearchPlugin)