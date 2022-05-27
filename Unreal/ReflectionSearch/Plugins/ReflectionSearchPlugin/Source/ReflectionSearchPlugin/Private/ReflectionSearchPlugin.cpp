// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReflectionSearchPlugin.h"
#include "ReflectionSearchPluginStyle.h"
#include "ReflectionSearchPluginCommands.h"
#include "ReflectionSearchTool.h"
#include "LevelEditor.h"
#include "LevelEditorMenuContext.h"
#include "Widgets/Docking/SDockTab.h"

#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Editor/EditorEngine.h"
#include "Engine/Selection.h"

DEFINE_LOG_CATEGORY(ReflectionSearchPlugin);

extern UNREALED_API UEditorEngine* GEditor;

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

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FReflectionSearchPluginModule::RegisterMenus));

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ReflectionSearchPluginTabName,
	                                                  FOnSpawnTab::CreateRaw(
		                                                  this, &FReflectionSearchPluginModule::OnSpawnPluginTab))
	                        .SetDisplayName(LOCTEXT("FReflectionSearchPluginTabTitle", "ReflectionSearchPlugin"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	PluginCommands->MapAction(
		FReflectionSearchPluginCommands::Get().OpenSearchWindow,
		FExecuteAction::CreateRaw(this, &FReflectionSearchPluginModule::OnActorMenuClicked),
		FCanExecuteAction());
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

static void DisplayAllChildren(TSharedRef<SWidget> ParentWidget, FString ChildWidgetName)
{
	UE_LOG(ReflectionSearchPlugin, Error, TEXT(" %s"), *ParentWidget->GetType().ToString());
	const auto CheckWidgetFunc = [ChildWidgetName](TSharedRef<SWidget> Widget)
	{
		if (Widget->GetType().ToString() == ChildWidgetName)
		{
			// FoundChild = Widget.Get();
		}
		UE_LOG(ReflectionSearchPlugin, Error, TEXT(" %s"), *Widget->GetType().ToString());
	};
	int32 NumChildren = ParentWidget->GetChildren()->Num();
	for (int32 NodeIndex = 0; NodeIndex < NumChildren; ++NodeIndex)
	{
		// Check all the children of the given ParentWidget, but only track the index at the top level
		for (int32 ChildIdx = 0; ChildIdx < ParentWidget->GetChildren()->Num(); ++ChildIdx)
		{
			auto Child = ParentWidget->GetChildren()->GetChildAt(ChildIdx);

			if (Child->GetChildren()->Num() > 0)
			{
				DisplayAllChildren(Child, ChildWidgetName);
			}
			CheckWidgetFunc(ParentWidget->GetChildren()->GetChildAt(ChildIdx));
		}
	}
}

bool SearchWidget(TSharedPtr<SWidget> ParentWidget, TSharedPtr<SWidget>& FoundedChild, FString ChildWidgetName)
{
	if (ParentWidget->GetType().ToString().Equals(ChildWidgetName) )
	{
		FoundedChild = ParentWidget;
		return true;
	}
	for (int32 ChildIdx = 0; ChildIdx < ParentWidget->GetAllChildren()->Num(); ++ChildIdx)
	{
		auto Child = ParentWidget->GetAllChildren()->GetChildAt(ChildIdx);
		bool Found = SearchWidget(Child, FoundedChild, ChildWidgetName);
		if (Found)
		{
			break;
		}
	}
	return false;
}

void FReflectionSearchPluginModule::OnActorMenuClicked()
{
	auto Tab = FGlobalTabmanager::Get()->TryInvokeTab(ReflectionSearchPluginTabName);
	TSharedPtr<SWidget> ContentWidget = Tab->GetContent();

	FString ChildWidgetName = FString("ReflectionSearchTool");
	TSharedPtr<SWidget> FoundedChild;
	SearchWidget(ContentWidget, FoundedChild, ChildWidgetName);

	// DisplayAllChilds(Tab->GetContent(), ChildWidgetName);


	TSharedPtr<const ReflectionSearchTool> ReflectionSearchToolWidget = StaticCastSharedPtr<const
		ReflectionSearchTool>(FoundedChild);
	// TWeakObjectPtr<ReflectionSearchTool> NonConstEnumPtr = MakeWeakObjectPtr(const_cast<ReflectionSearchTool*>(ReflectionSearchToolWidget.Get()));
	auto* NonConstPtr = const_cast<ReflectionSearchTool*>(ReflectionSearchToolWidget.Get());
	UToolMenu* InMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.ActorContextMenu");
	// ULevelEditorContextMenuContext* LevelEditorContext = InMenu->FindContext<ULevelEditorContextMenuContext>();
	// if (!LevelEditorContext || !LevelEditorContext->LevelEditor.IsValid())
	// {
	// 	return;
	// }
	//
	// TWeakPtr<SLevelEditor> LevelEditor = LevelEditorContext->LevelEditor;

	// Generate information about our selection
	TArray<AActor*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);
	if (SelectedActors.Num() == 1 && NonConstPtr)
	{
		NonConstPtr->SetSearchStr(SelectedActors[0]->GetClass()->GetName());
		NonConstPtr->UpdateView();
	}
}


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
			Section.AddMenuEntryWithCommandList(FReflectionSearchPluginCommands::Get().OpenPluginWindow,
			                                    PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(FReflectionSearchPluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
	{
		//在场景的Actor的Menu的 ActorCode Section添加新的菜单项
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.ActorContextMenu");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("ActorCode");
			Section.AddMenuEntryWithCommandList(FReflectionSearchPluginCommands::Get().OpenSearchWindow,
			                                    PluginCommands);
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FReflectionSearchPluginModule, ReflectionSearchPlugin)
