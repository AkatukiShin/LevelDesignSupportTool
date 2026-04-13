// “Copyright (c) 2026, Kousuke Komiya All rights reserved.”

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/Ticker.h"
#include "LevelDesignSupportToolPresenter.generated.h"

class ULevelDesignSupportToolViewBase;
class ULevelDesignSupportToolModel;
class USelectedActorListItemObject;
class UTransformOffsetSettings;
class URandomOffsetSettings;

/**
 * 
 */
UCLASS(BlueprintType)
class LEVELDESIGNSUPPORTTOOL_API ULevelDesignSupportToolPresenter : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Presenter")
	void Initialize(ULevelDesignSupportToolViewBase* InView);

	UFUNCTION(BlueprintCallable, Category = "Presenter")
	void Shutdown();

private:
	// Handlers
	UFUNCTION()
	void HandleActorItemClicked(USelectedActorListItemObject* ClickedItem);

	UFUNCTION()
	void HandleApplyRequested();

	UFUNCTION()
	void HandlePreviewRequested();

	UFUNCTION()
	void HandleResetRequested();

	void HandleEditorSelectionChanged(UObject* NewSelection);
	void RequestRefreshSelectedActors();
	bool HandleDeferredRefresh(float DeltaTime);

private:
	void RefreshSelectedActors();
	void BindEditorSelectionChanged();
	void UnbindEditorSelectionChanged();

private:
	// Reference
	UPROPERTY()
	TObjectPtr<ULevelDesignSupportToolViewBase> View;

	UPROPERTY()
	TObjectPtr<ULevelDesignSupportToolModel> Model;

	// State
	bool IsHandlingSelectionSync = false;
	bool bPendingRefreshSelectedActors = false;
	bool bRefreshScheduled = false;

	FTSTicker::FDelegateHandle RefreshTickerHandle;
};
