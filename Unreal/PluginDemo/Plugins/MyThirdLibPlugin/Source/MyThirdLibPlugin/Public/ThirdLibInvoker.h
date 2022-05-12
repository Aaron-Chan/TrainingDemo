#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "ThirdLibInvoker.generated.h"
/**
 *
 */
UCLASS()
class  MYTHIRDLIBPLUGIN_API UThirdLibInvoker : public UObject
{
	GENERATED_BODY()

		/** Handle to the test dll we will load */
	void* ExampleLibraryHandle;
public:
	void InvokeLib();
	~UThirdLibInvoker();
};
