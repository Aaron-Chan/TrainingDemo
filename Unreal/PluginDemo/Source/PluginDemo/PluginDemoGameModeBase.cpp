// Copyright Epic Games, Inc. All Rights Reserved.


#include "PluginDemoGameModeBase.h"
#include "ThirdLibInvoker.h"
#include "MyClass.h"

void APluginDemoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UThirdLibInvoker* lib = NewObject<UThirdLibInvoker>();
	lib->InvokeLib();

	MyClass myClass;
	myClass.GetNumber();
}
