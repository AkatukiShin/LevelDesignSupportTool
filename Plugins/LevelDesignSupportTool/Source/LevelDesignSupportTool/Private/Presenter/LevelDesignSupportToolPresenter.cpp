// “Copyright (c) 2026, Kousuke Komiya All rights reserved.”


#include "Presenter/LevelDesignSupportToolPresenter.h"
#include "View/LevelDesignSupportToolViewBase.h"
#include "Model/LevelDesignSupportToolModel.h"
#include "Data/TransformOffsetSettings.h"
#include "Data/SelectedActorListItemObject.h"
#include "Engine/Selection.h"
#include "Editor.h"

#pragma region Lifecycle
void ULevelDesignSupportToolPresenter::Initialize(ULevelDesignSupportToolViewBase* InView)
{
	View = InView;

	// Offset処理等を行うMedelの作成
	if (!Model)
	{
		Model = NewObject<ULevelDesignSupportToolModel>(this);
	}

	// イベント登録
	if (View)
	{
		View->OnActorItemClicked.AddDynamic(
			this, 
			&ULevelDesignSupportToolPresenter::HandleActorItemClicked
		);
		View->OnApplyButtonClicked.AddDynamic(
			this,
			&ULevelDesignSupportToolPresenter::HandleApplyRequested
		);
		View->OnPreviewButtonClicked.AddDynamic(
			this,
			&ULevelDesignSupportToolPresenter::HandlePreviewRequested
		);

		View->OnResetButtonCliked.AddDynamic(
			this,
			&ULevelDesignSupportToolPresenter::HandleResetRequested
		);
	}

	BindEditorSelectionChanged();
	RefreshSelectedActors();
}

/// <summary>
/// Plugin終了時に初期化を行う関数
/// </summary>
void ULevelDesignSupportToolPresenter::Shutdown()
{
	UE_LOG(LogTemp, Log, TEXT("LevelDesignSupportPlugin Shutdown"));

	if (RefreshTickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(RefreshTickerHandle);
		RefreshTickerHandle.Reset();
	}

	bRefreshScheduled = false;
	bPendingRefreshSelectedActors = false;

	Model->ClearPreview();
	UnbindEditorSelectionChanged();
}
#pragma endregion

#pragma region EventHandlers
void ULevelDesignSupportToolPresenter::HandleActorItemClicked(USelectedActorListItemObject* ClickedItem)
{
	if (!Model || !ClickedItem) return;

	IsHandlingSelectionSync = true;
	Model->SelectActorInEditor(ClickedItem->GetActor());
	IsHandlingSelectionSync = false;
}

void ULevelDesignSupportToolPresenter::HandleApplyRequested()
{
	if (!View || !Model)	return;

	Model->ApplyPreview(
		View->GetTransformOffsetSettings(),
		View->GetRandomOffsetSettings()
	);

	RefreshSelectedActors();
}

void ULevelDesignSupportToolPresenter::HandlePreviewRequested()
{
	if (!View || !Model )	return;

	Model->BuildCombinedPreview(
		View->GetTransformOffsetSettings(),
		View->GetRandomOffsetSettings()
	);

	RefreshSelectedActors();
}

void ULevelDesignSupportToolPresenter::HandleResetRequested()
{
	if (!View)	return;
	Model->ClearPreview();
}
#pragma endregion

#pragma region EditorSelection
void ULevelDesignSupportToolPresenter::HandleEditorSelectionChanged(UObject* NewSelection)
{
	if (IsHandlingSelectionSync)
	{
		return;
	}

	if (!View || !Model)
	{
		return;
	}

	RequestRefreshSelectedActors();
}

bool ULevelDesignSupportToolPresenter::HandleDeferredRefresh(float DeltaTime)
{
	bRefreshScheduled = false;
	RefreshTickerHandle.Reset();

	if (!bPendingRefreshSelectedActors)
	{
		return false;
	}

	bPendingRefreshSelectedActors = false;

	if (!View || !Model)
	{
		return false;
	}

	RefreshSelectedActors();
	return false;
}

void ULevelDesignSupportToolPresenter::BindEditorSelectionChanged()
{
#if WITH_EDITOR
	USelection::SelectionChangedEvent.AddUObject(
		this,
		&ULevelDesignSupportToolPresenter::HandleEditorSelectionChanged
	);
#endif
}

void ULevelDesignSupportToolPresenter::UnbindEditorSelectionChanged()
{
#if WITH_EDITOR
	USelection::SelectionChangedEvent.RemoveAll(this);
#endif
}

void ULevelDesignSupportToolPresenter::RequestRefreshSelectedActors()
{
	bPendingRefreshSelectedActors = true;

	if (bRefreshScheduled)
	{
		return;
	}

	bRefreshScheduled = true;

	RefreshTickerHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &ULevelDesignSupportToolPresenter::HandleDeferredRefresh)
	);
}
#pragma endregion

#pragma region ViewRefresh
void ULevelDesignSupportToolPresenter::RefreshSelectedActors()
{
	if (!View || !Model)
	{
		return;
	}

	const TArray<FSelectedActorInfo> SelectedActors = Model->GetSelectedActorsInfo();

	View->ShowSelectedActors(SelectedActors);
}
#pragma endregion