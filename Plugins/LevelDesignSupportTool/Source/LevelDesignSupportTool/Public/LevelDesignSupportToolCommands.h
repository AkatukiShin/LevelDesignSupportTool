// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "LevelDesignSupportToolStyle.h"

class FLevelDesignSupportToolCommands : public TCommands<FLevelDesignSupportToolCommands>
{
public:

	FLevelDesignSupportToolCommands()
		: TCommands<FLevelDesignSupportToolCommands>(TEXT("LevelDesignSupportTool"), NSLOCTEXT("Contexts", "LevelDesignSupportTool", "LevelDesignSupportTool Plugin"), NAME_None, FLevelDesignSupportToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};