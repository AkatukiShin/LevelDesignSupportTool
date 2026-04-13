// “Copyright (c) 2026, Kousuke Komiya All rights reserved.”


#include "View/LevelDesignSupportToolViewBase.h"
#include "Data/TransformOffsetSettings.h"
#include "Data/SelectedActorListItemObject.h"
#include "Data/SelectedActorInfo.h"
#include "Data/RandomOffsetSettings.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/SpinBox.h"
#include "ScopedTransaction.h"

#pragma region Initialize
void ULevelDesignSupportToolViewBase::NativeInitialize()
{
	if (!TransformOffsetSettings)
	{
		TransformOffsetSettings = NewObject<UTransformOffsetSettings>(this);
		TransformOffsetSettings->SetFlags(RF_Transactional);
	}
	if (!RandomOffsetSettings)
	{
		RandomOffsetSettings = NewObject<URandomOffsetSettings>(this);
		RandomOffsetSettings->SetFlags(RF_Transactional);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleApplyOffsetButtonClicked
		);
	}

	if (PreviewButton)
	{
		PreviewButton->OnClicked.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandlePreviewButtonClicked
		);
	}

	if (ResetButton)
	{
		ResetButton->OnClicked.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleResetButtonClicked
		);
	}

	// Location
	if (LocationXMinusButton) LocationXMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedLocationXMinusButton);
	if (LocationXPlusButton)  LocationXPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedLocationXPlusButton);
	if (LocationYMinusButton) LocationYMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedLocationYMinusButton);
	if (LocationYPlusButton)  LocationYPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedLocationYPlusButton);
	if (LocationZMinusButton) LocationZMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedLocationZMinusButton);
	if (LocationZPlusButton)  LocationZPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedLocationZPlusButton);

	// Rotation
	if (RotationXMinusButton) RotationXMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedRotationXMinusButton);
	if (RotationXPlusButton)  RotationXPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedRotationXPlusButton);
	if (RotationYMinusButton) RotationYMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedRotationYMinusButton);
	if (RotationYPlusButton)  RotationYPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedRotationYPlusButton);
	if (RotationZMinusButton) RotationZMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedRotationZMinusButton);
	if (RotationZPlusButton)  RotationZPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedRotationZPlusButton);

	// Scale
	if (ScaleXMinusButton) ScaleXMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedScaleXMinusButton);
	if (ScaleXPlusButton)  ScaleXPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedScaleXPlusButton);
	if (ScaleYMinusButton) ScaleYMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedScaleYMinusButton);
	if (ScaleYPlusButton)  ScaleYPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedScaleYPlusButton);
	if (ScaleZMinusButton) ScaleZMinusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedScaleZMinusButton);
	if (ScaleZPlusButton)  ScaleZPlusButton->OnClicked.AddDynamic(this, &ULevelDesignSupportToolViewBase::OnClickedScaleZPlusButton);

	if (SelectedActorsList)
	{
		SelectedActorsList->OnItemClicked().AddUObject(
			this,
			&ULevelDesignSupportToolViewBase::HandleSelectedActorItemClicked
		);
	}

	if (LocationOffsetCheckBox)
	{
		LocationOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

		LocationOffsetCheckBox->OnCheckStateChanged.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleLocationCheckChanged
		);
	}

	if (RotationOffsetCheckBox)
	{
		RotationOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

		RotationOffsetCheckBox->OnCheckStateChanged.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleRotationCheckChanged
		);
	}

	if (ScaleOffsetCheckBox)
	{
		ScaleOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

		ScaleOffsetCheckBox->OnCheckStateChanged.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleScaleCheckChanged
		);
	}

	if (LocationRandomOffsetCheckBox)
	{
		LocationRandomOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

		LocationRandomOffsetCheckBox->OnCheckStateChanged.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleLocationRandomCheckChanged
		);
	}

	if (RotationRandomOffsetCheckBox)
	{
		RotationRandomOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

		RotationRandomOffsetCheckBox->OnCheckStateChanged.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleRotationRandomCheckChanged
		);
	}

	if (ScaleRandomOffsetCheckBox)
	{
		ScaleRandomOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

		ScaleRandomOffsetCheckBox->OnCheckStateChanged.AddDynamic(
			this,
			&ULevelDesignSupportToolViewBase::HandleScaleRandomCheckChanged
		);
	}
}

void ULevelDesignSupportToolViewBase::InputValueInitialize()
{
	TransformOffsetSettings->LocationOffset = FVector::ZeroVector;
	TransformOffsetSettings->RotationOffset = FRotator::ZeroRotator;
	TransformOffsetSettings->ScaleOffset = FVector::ZeroVector;

	RandomOffsetSettings->LocationXOffset = FVector2D::ZeroVector;
	RandomOffsetSettings->LocationYOffset = FVector2D::ZeroVector;
	RandomOffsetSettings->LocationZOffset = FVector2D::ZeroVector;

	RandomOffsetSettings->RotationXOffset = FVector2D::ZeroVector;
	RandomOffsetSettings->RotationYOffset = FVector2D::ZeroVector;
	RandomOffsetSettings->RotationZOffset = FVector2D::ZeroVector;

	RandomOffsetSettings->ScaleXOffset = FVector2D::ZeroVector;
	RandomOffsetSettings->ScaleYOffset = FVector2D::ZeroVector;
	RandomOffsetSettings->ScaleZOffset = FVector2D::ZeroVector;

	LocationOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);
	RotationOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);
	ScaleOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

	LocationRandomOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);
	RotationRandomOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);
	ScaleRandomOffsetCheckBox->SetCheckedState(ECheckBoxState::Checked);

	LocationStepSpinBox->SetValue(0.0f);
	RotationStepSpinBox->SetValue(0.0f);
	ScaleStepSpinBox->SetValue(0.0f);
}
#pragma endregion

#pragma region View Uppdate
void ULevelDesignSupportToolViewBase::ShowSelectedActors(const TArray<FSelectedActorInfo>& ActorsInfo)
{
	SelectedActorsList->ClearListItems();
	PreviewActorsList->ClearListItems();

	for (const FSelectedActorInfo ActorInfo : ActorsInfo)
	{
		TObjectPtr<USelectedActorListItemObject> SelectedActorListItemObject = NewObject<USelectedActorListItemObject>(this);

		SelectedActorListItemObject->Setup(ActorInfo);

		SelectedActorsList->AddItem(SelectedActorListItemObject);
		
		if(ActorInfo.HasPreview)	PreviewActorsList->AddItem(SelectedActorListItemObject);
	}
}
#pragma endregion

#pragma region Handler
void ULevelDesignSupportToolViewBase::HandleLocationCheckChanged(bool IsChecked)
{
	if (TransformOffsetSettings)
	{
		const FScopedTransaction Transaction(
			NSLOCTEXT("LevelDesignSupportToolViewBase", "LocationOffsetCheck", "LocationOffsetCheck")
		);

		TransformOffsetSettings->Modify();
		TransformOffsetSettings->ApplyLocation = IsChecked;
	}
}

void ULevelDesignSupportToolViewBase::HandleRotationCheckChanged(bool IsChecked)
{
	if (TransformOffsetSettings)
	{
		const FScopedTransaction Transaction(
			NSLOCTEXT("LevelDesignSupportToolViewBase", "RotationOffsetCheck", "RotationOffsetCheck")
		);

		TransformOffsetSettings->Modify();
		TransformOffsetSettings->ApplyRotation = IsChecked;
	}
}

void ULevelDesignSupportToolViewBase::HandleScaleCheckChanged(bool IsChecked)
{
	if (TransformOffsetSettings)
	{
		const FScopedTransaction Transaction(
			NSLOCTEXT("LevelDesignSupportToolViewBase", "ScaleOffsetCheck", "ScaleOffsetCheck")
		);

		TransformOffsetSettings->Modify();
		TransformOffsetSettings->ApplyScale = IsChecked;
	}
}

void ULevelDesignSupportToolViewBase::HandleLocationRandomCheckChanged(bool IsChecked)
{
	if (RandomOffsetSettings)
	{
		const FScopedTransaction Transaction(
			NSLOCTEXT("LevelDesignSupportToolViewBase", "LocationRandomCheck", "LocationRandomCheck")
		);

		RandomOffsetSettings->Modify();
		RandomOffsetSettings->ApplyLocation = IsChecked;
	}
}

void ULevelDesignSupportToolViewBase::HandleRotationRandomCheckChanged(bool IsChecked)
{
	if (RandomOffsetSettings)
	{
		const FScopedTransaction Transaction(
			NSLOCTEXT("LevelDesignSupportToolViewBase", "RotationRandomCheck", "RotationRandomCheck")
		);

		RandomOffsetSettings->Modify();
		RandomOffsetSettings->ApplyRotation = IsChecked;
	}
}

void ULevelDesignSupportToolViewBase::HandleScaleRandomCheckChanged(bool IsChecked)
{
	if (RandomOffsetSettings)
	{
		const FScopedTransaction Transaction(
			NSLOCTEXT("LevelDesignSupportToolViewBase", "ScaleRandomCheck", "ScaleRandomCheck")
		);

		RandomOffsetSettings->Modify();
		RandomOffsetSettings->ApplyScale = IsChecked;
	}
}

void ULevelDesignSupportToolViewBase::HandleSelectedActorItemClicked(UObject* ClickedItem)
{
	USelectedActorListItemObject* ItemObject = Cast<USelectedActorListItemObject>(ClickedItem);
	if (!ItemObject)
	{
		return;
	}

	OnActorItemClicked.Broadcast(ItemObject);
}

void ULevelDesignSupportToolViewBase::HandleApplyOffsetButtonClicked()
{
	OnApplyButtonClicked.Broadcast();
}

void ULevelDesignSupportToolViewBase::HandlePreviewButtonClicked()
{
	OnPreviewButtonClicked.Broadcast();
}

void ULevelDesignSupportToolViewBase::HandleResetButtonClicked()
{
	const FScopedTransaction Transaction(
		NSLOCTEXT("LevelDesignSupportToolViewBase", "ResetInputs", "ResetToolInputs")
	);

	if (TransformOffsetSettings)
	{
		TransformOffsetSettings->Modify();
	}

	if (RandomOffsetSettings)
	{
		RandomOffsetSettings->Modify();
	}

	OnResetButtonCliked.Broadcast();
	InputValueInitialize();
}
#pragma endregion

#pragma region Utility
void ULevelDesignSupportToolViewBase::AddLocationOffset(const FVector& Delta)
{
	if (!TransformOffsetSettings)
	{
		return;
	}
	const FScopedTransaction Transaction(
		NSLOCTEXT("LevelDesignSupportToolViewBase", "ChangeLocationOffset", "ChangeLocationOffset")
	);

	TransformOffsetSettings->Modify();
	TransformOffsetSettings->LocationOffset += Delta;
}

void ULevelDesignSupportToolViewBase::AddRotationOffset(const FRotator& Delta)
{
	if (!TransformOffsetSettings)
	{
		return;
	}
	const FScopedTransaction Transaction(
		NSLOCTEXT("LevelDesignSupportToolViewBase", "ChangeRotationOffset", "ChangeRotationOffset")
	);

	TransformOffsetSettings->Modify();
	TransformOffsetSettings->RotationOffset += Delta;
}

void ULevelDesignSupportToolViewBase::AddScaleOffset(const FVector& Delta)
{
	if (!TransformOffsetSettings)
	{
		return;
	}
	const FScopedTransaction Transaction(
		NSLOCTEXT("LevelDesignSupportToolViewBase", "ChangeScaleOffset", "ChangeScaleOffset")
	);

	TransformOffsetSettings->Modify();
	TransformOffsetSettings->ScaleOffset += Delta;
}

float ULevelDesignSupportToolViewBase::GetLocationStep() const
{
	if (!LocationStepSpinBox)	return 0.0f;

	return LocationStepSpinBox->GetValue();
}

float ULevelDesignSupportToolViewBase::GetRotationStep() const
{
	if (!RotationStepSpinBox)	return 0.0f;

	return RotationStepSpinBox->GetValue();
}

float ULevelDesignSupportToolViewBase::GetScaleStep() const
{
	if (!ScaleStepSpinBox)	return 0.0f;

	return ScaleStepSpinBox->GetValue();
}
#pragma endregion

#pragma region Button Clicked
void ULevelDesignSupportToolViewBase::OnClickedLocationXMinusButton()
{
	AddLocationOffset(FVector(-GetLocationStep(), 0.0f, 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedLocationXPlusButton()
{
	AddLocationOffset(FVector(GetLocationStep(), 0.0f, 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedLocationYMinusButton()
{
	AddLocationOffset(FVector(0.0f, -GetLocationStep(), 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedLocationYPlusButton()
{
	AddLocationOffset(FVector(0.0f, GetLocationStep(), 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedLocationZMinusButton()
{
	AddLocationOffset(FVector(0.0f, 0.0f, -GetLocationStep()));
}

void ULevelDesignSupportToolViewBase::OnClickedLocationZPlusButton()
{
	AddLocationOffset(FVector(0.0f, 0.0f, GetLocationStep()));
}

void ULevelDesignSupportToolViewBase::OnClickedRotationXMinusButton()
{
	AddRotationOffset(FRotator(0.0f, 0.0f, -GetRotationStep()));
}

void ULevelDesignSupportToolViewBase::OnClickedRotationXPlusButton()
{
	AddRotationOffset(FRotator(0.0f, 0.0f, GetRotationStep()));
}

void ULevelDesignSupportToolViewBase::OnClickedRotationYMinusButton()
{
	AddRotationOffset(FRotator(-GetRotationStep(), 0.0f, 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedRotationYPlusButton()
{
	AddRotationOffset(FRotator(GetRotationStep(), 0.0f, 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedRotationZMinusButton()
{
	AddRotationOffset(FRotator(0.0f, -GetRotationStep(), 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedRotationZPlusButton()
{
	AddRotationOffset(FRotator(0.0f, GetRotationStep(), 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedScaleXMinusButton()
{
	AddScaleOffset(FVector(-GetScaleStep(), 0.0f, 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedScaleXPlusButton()
{
	AddScaleOffset(FVector(GetScaleStep(), 0.0f, 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedScaleYMinusButton()
{
	AddScaleOffset(FVector(0.0f, -GetScaleStep(), 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedScaleYPlusButton()
{
	AddScaleOffset(FVector(0.0f, GetScaleStep(), 0.0f));
}

void ULevelDesignSupportToolViewBase::OnClickedScaleZMinusButton()
{
	AddScaleOffset(FVector(0.0f, 0.0f, -GetScaleStep()));
}

void ULevelDesignSupportToolViewBase::OnClickedScaleZPlusButton()
{
	AddScaleOffset(FVector(0.0f, 0.0f, GetScaleStep()));
}
#pragma endregion
