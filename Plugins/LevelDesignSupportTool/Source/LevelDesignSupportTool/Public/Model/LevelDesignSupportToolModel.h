// “Copyright (c) 2026, Kousuke Komiya All rights reserved.”

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Data/SelectedActorInfo.h"
#include "Data/ActorPreviewData.h"
#include "LevelDesignSupportToolModel.generated.h"

class UTransformOffsetSettings;
class URandomOffsetSettings;
class UMaterialInterface;

/**
 * 
 */
UCLASS()
class LEVELDESIGNSUPPORTTOOL_API ULevelDesignSupportToolModel : public UObject
{
	GENERATED_BODY()

public:
	// Array
	TArray<AActor*> GetSelectedActors();
	TArray<FSelectedActorInfo> GetSelectedActorsInfo();

	// Commands
	void SelectActorInEditor(AActor* ActorToSelect);
	void BuildCombinedPreview(
		const UTransformOffsetSettings* TransformSettings,
		const URandomOffsetSettings* RandomSettings
	);
	void ApplyPreview(
		const UTransformOffsetSettings* TransformSettings,
		const URandomOffsetSettings* RandomSettings
	);
	void ClearPreview();

private:
	// PreviewHelpers
	void InitializePreviewMaterial();
	AActor* SpawnPreviewActor(AActor* SourceActor, const FTransform& PreviewTransform);
private:
	// Array
	TArray<AActor*> SelectedActors;
	TArray<FSelectedActorInfo> SelectedActorsInfo;
	TArray<FActorPreviewData> PreviewDatas;

	TObjectPtr<UMaterialInterface> PreviewMaterial = nullptr;

	bool HasPreviewDatas = false;
};
