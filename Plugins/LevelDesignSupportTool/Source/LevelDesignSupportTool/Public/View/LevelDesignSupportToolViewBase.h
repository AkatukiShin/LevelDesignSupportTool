// “Copyright (c) 2026, Kousuke Komiya All rights reserved.”

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Components/ListView.h"
#include "LevelDesignSupportToolViewBase.generated.h"

class UButton;
class UListView;
class UCheckBox;
class USpinBox;
class USelectedActorListItemObject;
class UTransformOffsetSettings;
class URandomOffsetSettings;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorItemClicked, USelectedActorListItemObject*, ClickedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetButtonClicled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreviewButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApplyButtonClicked);

UCLASS()
class LEVELDESIGNSUPPORTTOOL_API ULevelDesignSupportToolViewBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	// Lifecycle
	UFUNCTION(BlueprintCallable, Category = "ViewBase")
	void NativeInitialize();

	// Accessors
	UFUNCTION(BlueprintCallable, Category="View")
	UTransformOffsetSettings* GetTransformOffsetSettings() const
	{
		return TransformOffsetSettings;
	}

	UFUNCTION(BlueprintCallable, Category="View")
	URandomOffsetSettings* GetRandomOffsetSettings() const
	{
		return RandomOffsetSettings;
	}

	// ViewUpdate
	UFUNCTION(Category="View")
	void ShowSelectedActors(const TArray<FSelectedActorInfo>& ActorsInfo);

	// Events
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnActorItemClicked OnActorItemClicked;

	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnApplyButtonClicked OnApplyButtonClicked;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnPreviewButtonClicked OnPreviewButtonClicked;

	UPROPERTY(BlueprintAssignable, Category="Event")
	FOnResetButtonClicled OnResetButtonCliked;

protected:
	// ListViews
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> SelectedActorsList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> PreviewActorsList;

	// ControlButons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PreviewButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ApplyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ResetButton;

	// TransformButtons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LocationXMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LocationXPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LocationYMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LocationYPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LocationZMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LocationZPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RotationXMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RotationXPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RotationYMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RotationYPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RotationZMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RotationZPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ScaleXMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ScaleXPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ScaleYMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ScaleYPlusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ScaleZMinusButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ScaleZPlusButton;

	// SpinBoxes
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> LocationStepSpinBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> RotationStepSpinBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USpinBox> ScaleStepSpinBox;

	// CheckBoxes
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> LocationOffsetCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> RotationOffsetCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> ScaleOffsetCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> LocationRandomOffsetCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> RotationRandomOffsetCheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> ScaleRandomOffsetCheckBox;

	// Settings
	UPROPERTY(BlueprintReadOnly, Category="Settings")
	TObjectPtr<UTransformOffsetSettings> TransformOffsetSettings;

	UPROPERTY(BlueprintReadOnly, Category="Settings")
	TObjectPtr<URandomOffsetSettings> RandomOffsetSettings;

private:
	// Initialization
	UFUNCTION()
	void InputValueInitialize();

	// Handler
	UFUNCTION()
	void HandleLocationCheckChanged(bool IsChecked);

	UFUNCTION()
	void HandleRotationCheckChanged(bool IsChecked);

	UFUNCTION()
	void HandleScaleCheckChanged(bool IsChecked);

	UFUNCTION()
	void HandleLocationRandomCheckChanged(bool IsChecked);

	UFUNCTION()
	void HandleRotationRandomCheckChanged(bool IsChecked);

	UFUNCTION()
	void HandleScaleRandomCheckChanged(bool IsChecked);

	UFUNCTION()
	void HandleSelectedActorItemClicked(UObject* ClickedItem);

	UFUNCTION()
	void HandleApplyOffsetButtonClicked();

	UFUNCTION()
	void HandlePreviewButtonClicked();

	UFUNCTION()
	void HandleResetButtonClicked();

	// Utility
	void AddLocationOffset(const FVector& Delta);
	void AddRotationOffset(const FRotator& Delta);
	void AddScaleOffset(const FVector& Delta);

	float GetLocationStep() const;
	float GetRotationStep() const;
	float GetScaleStep() const;

	// ButtonLocation
	UFUNCTION()
	void OnClickedLocationXMinusButton();
	UFUNCTION()
	void OnClickedLocationXPlusButton();
	UFUNCTION()
	void OnClickedLocationYMinusButton();
	UFUNCTION()
	void OnClickedLocationYPlusButton();
	UFUNCTION()
	void OnClickedLocationZMinusButton();
	UFUNCTION()
	void OnClickedLocationZPlusButton();

	// ButtonRotation
	UFUNCTION()
	void OnClickedRotationXMinusButton();
	UFUNCTION()
	void OnClickedRotationXPlusButton();
	UFUNCTION()
	void OnClickedRotationYMinusButton();
	UFUNCTION()
	void OnClickedRotationYPlusButton();
	UFUNCTION()
	void OnClickedRotationZMinusButton();
	UFUNCTION()
	void OnClickedRotationZPlusButton();

	// ButtonScale
	UFUNCTION()
	void OnClickedScaleXMinusButton();
	UFUNCTION()
	void OnClickedScaleXPlusButton();
	UFUNCTION()
	void OnClickedScaleYMinusButton();
	UFUNCTION()
	void OnClickedScaleYPlusButton();
	UFUNCTION()
	void OnClickedScaleZMinusButton();
	UFUNCTION()
	void OnClickedScaleZPlusButton();
};
