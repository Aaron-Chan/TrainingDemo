#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "ThirdLibInvoker2.generated.h"
/**
 *
 */
UCLASS()
class  UThirdLibInvoker2 : public UObject
{
	GENERATED_BODY()

		/** Handle to the test dll we will load */
	void* ExampleLibraryHandle;
public:
	
};
