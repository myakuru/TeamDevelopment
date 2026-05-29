// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectNull.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FProject_ProjectNullModule, ProjectNull, "ProjectNull" );

void FProject_ProjectNullModule::StartupModule()
{
	FString ShaderDir = FPaths::Combine(FPaths::ProjectDir(), "Shaders");
	AddShaderSourceDirectoryMapping("/ProjectNull", ShaderDir);
}

void FProject_ProjectNullModule::ShutdownModule()
{
}