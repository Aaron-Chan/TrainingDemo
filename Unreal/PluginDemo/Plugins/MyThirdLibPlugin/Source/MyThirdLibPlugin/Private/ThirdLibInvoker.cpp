
#include "ThirdLibInvoker.h"
#include "Core.h"
#include "Interfaces/IPluginManager.h"
#include "MyThirdLibPluginLibrary/ExampleLibrary.h"


#define LOCTEXT_NAMESPACE "UThirdLibInvoker"

void UThirdLibInvoker::InvokeLib()
{
	// Get the base directory of this plugin
	if (ExampleLibraryHandle == nullptr)
	{
		// Get the base directory of this plugin
		FString BaseDir = IPluginManager::Get().FindPlugin("MyThirdLibPlugin")->GetBaseDir();

		// Add on the relative location of the third party dll and load it
		FString LibraryPath;
#if PLATFORM_WINDOWS
		LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/MyThirdLibPluginLibrary/Win64/ExampleLibrary.dll"));
#elif PLATFORM_MAC
		LibraryPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/MyThirdLibPluginLibrary/Mac/Release/libExampleLibrary.dylib"));
#endif // PLATFORM_WINDOWS

		ExampleLibraryHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
	}
	if (ExampleLibraryHandle)
	{
		// Call the test function in the third party library that opens a message box
		ExampleLibraryFunction();
	}
	else
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("ThirdPartyLibraryError", "Failed to load example third party library"));
	}
}

UThirdLibInvoker::~UThirdLibInvoker()
{
	FPlatformProcess::FreeDllHandle(ExampleLibraryHandle);
	ExampleLibraryHandle = nullptr;
}
