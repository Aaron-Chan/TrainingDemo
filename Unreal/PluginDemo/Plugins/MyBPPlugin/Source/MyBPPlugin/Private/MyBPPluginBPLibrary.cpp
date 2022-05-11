// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyBPPluginBPLibrary.h"
#include "MyBPPlugin.h"

UMyBPPluginBPLibrary::UMyBPPluginBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float UMyBPPluginBPLibrary::MyBPPluginSampleFunction(float Param)
{
	return -1;
}

