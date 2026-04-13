// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelDesignSupportToolStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FLevelDesignSupportToolStyle::StyleInstance = nullptr;

void FLevelDesignSupportToolStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FLevelDesignSupportToolStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FLevelDesignSupportToolStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("LevelDesignSupportToolStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FLevelDesignSupportToolStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("LevelDesignSupportToolStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("LevelDesignSupportTool")->GetBaseDir() / TEXT("Resources"));

	Style->Set("LevelDesignSupportTool.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FLevelDesignSupportToolStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FLevelDesignSupportToolStyle::Get()
{
	return *StyleInstance;
}
