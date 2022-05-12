// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorToolbar.h"
#include "MyEditorToolbarStyle.h"
#include "MyEditorToolbarCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "LevelEditor.h"

static const FName MyEditorToolbarTabName("MyEditorToolbar");

#define LOCTEXT_NAMESPACE "FMyEditorToolbarModule"

void FMyEditorToolbarModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMyEditorToolbarStyle::Initialize();
	FMyEditorToolbarStyle::ReloadTextures();

	FMyEditorToolbarCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMyEditorToolbarCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMyEditorToolbarModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMyEditorToolbarModule::RegisterMenus));
}

void FMyEditorToolbarModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMyEditorToolbarStyle::Shutdown();

	FMyEditorToolbarCommands::Unregister();
}

void FMyEditorToolbarModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FMyEditorToolbarModule::PluginButtonClicked()")),
							FText::FromString(TEXT("MyEditorToolbar.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FMyEditorToolbarModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMyEditorToolbarCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMyEditorToolbarCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
	{
		TSharedPtr<FExtender> ToolExtender = MakeShareable(new FExtender());
		ToolExtender->AddToolBarExtension("Content", EExtensionHook::After, PluginCommands,
			FToolBarExtensionDelegate::CreateRaw(
				this, &FMyEditorToolbarModule::AddToolBarExtension));
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolExtender);
	}

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuBarExtension("Help", EExtensionHook::After, PluginCommands,
			FMenuBarExtensionDelegate::CreateRaw(
				this, &FMyEditorToolbarModule::AddMenuBarExtension));
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{

		UToolMenus* ToolMenus = UToolMenus::Get();
		UToolMenu* MyMenu = ToolMenus->RegisterMenu("LevelEditor.MainMenu.MySubMenu");

		FToolMenuSection& Section = MyMenu->AddSection("MySection0");
		Section.AddMenuEntryWithCommandList(FMyEditorToolbarCommands::Get().PluginAction, PluginCommands);
		UToolMenu* MenuBar = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
		MenuBar->AddSubMenu(
			"MainMenu",
			"MySection",
			"MySubMenu",
			LOCTEXT("MyMenu", "My")
		);
	}

	
}

void FMyEditorToolbarModule::AddToolBarExtension(class FToolBarBuilder& Builder)
{
	Builder.BeginSection(TEXT("MyButton2"));
	Builder.AddToolBarButton(FMyEditorToolbarCommands::Get().PluginAction);
	Builder.EndSection();
}

void FMyEditorToolbarModule::AddMenuBarExtension(class FMenuBarBuilder& Builder)
{
	Builder.AddMenuEntry(FMyEditorToolbarCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMyEditorToolbarModule, MyEditorToolbar)