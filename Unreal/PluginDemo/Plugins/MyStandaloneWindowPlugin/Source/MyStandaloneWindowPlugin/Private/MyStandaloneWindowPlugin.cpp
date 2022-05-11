// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyStandaloneWindowPlugin.h"
#include "MyStandaloneWindowPluginStyle.h"
#include "MyStandaloneWindowPluginCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName MyStandaloneWindowPluginTabName("MyStandaloneWindowPlugin");

#define LOCTEXT_NAMESPACE "FMyStandaloneWindowPluginModule"

void FMyStandaloneWindowPluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMyStandaloneWindowPluginStyle::Initialize();
	FMyStandaloneWindowPluginStyle::ReloadTextures();

	FMyStandaloneWindowPluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMyStandaloneWindowPluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMyStandaloneWindowPluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMyStandaloneWindowPluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MyStandaloneWindowPluginTabName, FOnSpawnTab::CreateRaw(this, &FMyStandaloneWindowPluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMyStandaloneWindowPluginTabTitle", "MyStandaloneWindowPlugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMyStandaloneWindowPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMyStandaloneWindowPluginStyle::Shutdown();

	FMyStandaloneWindowPluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MyStandaloneWindowPluginTabName);
}

TSharedRef<SDockTab> FMyStandaloneWindowPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FMyStandaloneWindowPluginModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("MyStandaloneWindowPlugin.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FMyStandaloneWindowPluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MyStandaloneWindowPluginTabName);
}

void FMyStandaloneWindowPluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMyStandaloneWindowPluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMyStandaloneWindowPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyStandaloneWindowPluginModule, MyStandaloneWindowPlugin)