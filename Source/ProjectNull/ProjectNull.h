// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FProject_ProjectNullModule : public FDefaultGameModuleImpl
{
public:

	virtual void StartupModule()override;
	virtual void ShutdownModule()override;
};
