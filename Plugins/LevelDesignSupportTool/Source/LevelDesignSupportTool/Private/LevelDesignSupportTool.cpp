// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelDesignSupportTool.h"
#include "LevelDesignSupportToolStyle.h"
#include "LevelDesignSupportToolCommands.h"
#include "ToolMenus.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

void FLevelDesignSupportToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLevelDesignSupportToolStyle::Initialize();
	FLevelDesignSupportToolStyle::ReloadTextures();

	FLevelDesignSupportToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLevelDesignSupportToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FLevelDesignSupportToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FLevelDesignSupportToolModule::RegisterMenus));
}

void FLevelDesignSupportToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FLevelDesignSupportToolStyle::Shutdown();

	FLevelDesignSupportToolCommands::Unregister();
}

void FLevelDesignSupportToolModule::PluginButtonClicked()
{
	ExecuteMenu();
}

void FLevelDesignSupportToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FLevelDesignSupportToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FLevelDesignSupportToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FLevelDesignSupportToolModule::ExecuteMenu()
{
	UObject* Blueprint = UEditorAssetLibrary::LoadAsset(FString(TEXT("/LevelDesignSupportTool/EditorUtilityWidget/EUW_LevelDesignTool")));
	if (IsValid(Blueprint))
	{
		UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(Blueprint);
		if (IsValid(EditorWidget))
		{
			UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
			EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidget);
		}
	}
}
	
IMPLEMENT_MODULE(FLevelDesignSupportToolModule, LevelDesignSupportTool)