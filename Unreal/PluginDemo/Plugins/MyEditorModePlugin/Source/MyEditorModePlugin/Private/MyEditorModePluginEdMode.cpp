// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyEditorModePluginEdMode.h"
#include "MyEditorModePluginEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"

const FEditorModeID FMyEditorModePluginEdMode::EM_MyEditorModePluginEdModeId = TEXT("EM_MyEditorModePluginEdMode");

FMyEditorModePluginEdMode::FMyEditorModePluginEdMode()
{

}

FMyEditorModePluginEdMode::~FMyEditorModePluginEdMode()
{

}

void FMyEditorModePluginEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FMyEditorModePluginEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}
}

void FMyEditorModePluginEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FMyEditorModePluginEdMode::UsesToolkits() const
{
	return true;
}




