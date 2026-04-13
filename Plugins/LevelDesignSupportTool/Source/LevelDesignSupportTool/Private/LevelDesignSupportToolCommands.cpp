// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelDesignSupportToolCommands.h"

#define LOCTEXT_NAMESPACE "FLevelDesignSupportToolModule"

void FLevelDesignSupportToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "LevelDesignSupportTool", "Bring up LevelDesignSupportTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
