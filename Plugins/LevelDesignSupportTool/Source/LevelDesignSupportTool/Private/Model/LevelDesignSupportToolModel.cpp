// “Copyright (c) 2026, Kousuke Komiya All rights reserved.”


#include "Model/LevelDesignSupportToolModel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/TransformOffsetSettings.h"
#include "Data/RandomOffsetSettings.h"
#include "ScopedTransaction.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Subsystems/EditorActorSubsystem.h"
#endif

#pragma region SelectedActors
TArray<AActor*> ULevelDesignSupportToolModel::GetSelectedActors()
{
	return SelectedActors;
}

TArray<FSelectedActorInfo> ULevelDesignSupportToolModel::GetSelectedActorsInfo()
{
	SelectedActorsInfo.Empty();
	SelectedActors.Empty();

#if WITH_EDITOR
	if (!GEditor)
	{
		return SelectedActorsInfo;
	}

	if (UEditorActorSubsystem* ActorSubSystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>())
	{
		SelectedActors = ActorSubSystem->GetSelectedLevelActors();
	}

	if (SelectedActors.IsEmpty())	return SelectedActorsInfo;

	for (AActor* SelectedActor : SelectedActors)
	{
		// CreateActorInfo
		FSelectedActorInfo Info;

		Info.ActorName = UKismetSystemLibrary::GetDisplayName(SelectedActor);
		Info.ActorClassName = SelectedActor->GetClass()->GetName();
		Info.Actor = SelectedActor;

		Info.CurrentLocation = SelectedActor->GetActorLocation();
		Info.CurrentRotation = SelectedActor->GetActorRotation();
		Info.CurrentScale = SelectedActor->GetActorScale3D();

		for (const FActorPreviewData& PreviewData : PreviewDatas)
		{
			if (PreviewData.SourceActor == SelectedActor)
			{
				Info.HasPreview = true;

				Info.PreviewAddLocation = PreviewData.AddedLocation;
				Info.PreviewAddRotation = PreviewData.AddedRotation;
				Info.PreviewAddScale = PreviewData.AddedScale;

				Info.PreviewNewLocation = PreviewData.PreviewTransform.GetLocation();
				Info.PreviewNewRotation = PreviewData.PreviewTransform.Rotator();
				Info.PreviewNewScale = PreviewData.PreviewTransform.GetScale3D();

				break;
			}
		}

		SelectedActorsInfo.Add(Info);
	}
#endif

	return SelectedActorsInfo;
}
#pragma endregion

#pragma region EditorSelection
void ULevelDesignSupportToolModel::SelectActorInEditor(AActor* ActorToSelect)
{
#if WITH_EDITOR
	if (!GEditor || !ActorToSelect)
	{
		return;
	}

	GEditor->SelectNone(true, true, false);
	GEditor->SelectActor(ActorToSelect, true, true, true);
	GEditor->NoteSelectionChange();
#endif
}
#pragma endregion

#pragma region PreviewBuild
void ULevelDesignSupportToolModel::BuildCombinedPreview(const UTransformOffsetSettings* TransformSettings, const URandomOffsetSettings* RandomSettings)
{
	ClearPreview(); 

	for (AActor* SelectedActor : SelectedActors)
	{
		if (!SelectedActor)
		{
			continue;
		}

		UStaticMeshComponent* StaticMeshComponent =
			Cast<UStaticMeshComponent>(SelectedActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (!StaticMeshComponent)
		{
			continue;
		}

		const FVector CurrentLocation = SelectedActor->GetActorLocation();
		const FRotator CurrentRotation = SelectedActor->GetActorRotation();
		const FVector CurrentScale = SelectedActor->GetActorScale3D();

		FVector AddLocation = FVector::ZeroVector;
		FRotator AddRotation = FRotator::ZeroRotator;
		FVector AddScale = FVector::ZeroVector;

		if (TransformSettings)
		{
			if (TransformSettings->ApplyLocation)
			{
				AddLocation += TransformSettings->LocationOffset;
			}

			if (TransformSettings->ApplyRotation)
			{
				AddRotation += TransformSettings->RotationOffset;
			}

			if (TransformSettings->ApplyScale)
			{
				AddScale += TransformSettings->ScaleOffset;
			}
		}

		if (RandomSettings)
		{
			if (RandomSettings->ApplyLocation)
			{
				const float LocationX = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->LocationXOffset.X, RandomSettings->LocationXOffset.Y);
				const float LocationY = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->LocationYOffset.X, RandomSettings->LocationYOffset.Y);
				const float LocationZ = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->LocationZOffset.X, RandomSettings->LocationZOffset.Y);

				AddLocation += FVector(LocationX, LocationY, LocationZ);
			}

			if (RandomSettings->ApplyRotation)
			{
				const float RotationX = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->RotationXOffset.X, RandomSettings->RotationXOffset.Y);
				const float RotationY = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->RotationYOffset.X, RandomSettings->RotationYOffset.Y);
				const float RotationZ = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->RotationZOffset.X, RandomSettings->RotationZOffset.Y);

				AddRotation += FRotator(RotationY, RotationZ, RotationX);
			}

			if (RandomSettings->ApplyScale)
			{
				const float ScaleX = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->ScaleXOffset.X, RandomSettings->ScaleXOffset.Y);
				const float ScaleY = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->ScaleYOffset.X, RandomSettings->ScaleYOffset.Y);
				const float ScaleZ = UKismetMathLibrary::RandomFloatInRange(
					RandomSettings->ScaleZOffset.X, RandomSettings->ScaleZOffset.Y);

				AddScale += FVector(ScaleX, ScaleY, ScaleZ);
			}
		}

		const FVector NewLocation = CurrentLocation + AddLocation;
		const FRotator NewRotation = CurrentRotation + AddRotation;
		const FVector NewScale = CurrentScale + AddScale;

		const FTransform PreviewTransform(NewRotation, NewLocation, NewScale);
		AActor* PreviewActor = SpawnPreviewActor(SelectedActor, PreviewTransform);

		if (!PreviewActor)
		{
			continue;
		}

		FActorPreviewData Data;
		Data.SourceActor = SelectedActor;
		Data.PreviewActor = PreviewActor;
		Data.CurrentTransform = SelectedActor->GetActorTransform();
		Data.PreviewTransform = PreviewTransform;
		Data.AddedLocation = AddLocation;
		Data.AddedRotation = AddRotation;
		Data.AddedScale = AddScale;

		PreviewDatas.Add(Data);
	}

	HasPreviewDatas = PreviewDatas.Num() > 0;
}

AActor* ULevelDesignSupportToolModel::SpawnPreviewActor(AActor* SourceActor, const FTransform& PreviewTransform)
{
#if WITH_EDITOR
	InitializePreviewMaterial();

	if (!SourceActor || !GEditor)
	{
		return nullptr;
	}

	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (!World)
	{
		return nullptr;
	}

	UStaticMeshComponent* SourceMeshComp =
		Cast<UStaticMeshComponent>(SourceActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

	if (!SourceMeshComp || !SourceMeshComp->GetStaticMesh())
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.ObjectFlags = RF_Transient;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AStaticMeshActor* PreviewActor = World->SpawnActor<AStaticMeshActor>(
		AStaticMeshActor::StaticClass(),
		PreviewTransform,
		SpawnParams
	);

	if (!PreviewActor)
	{
		return nullptr;
	}

	UStaticMeshComponent* PreviewMeshComp = PreviewActor->GetStaticMeshComponent();
	if (!PreviewMeshComp)
	{
		PreviewActor->Destroy();
		return nullptr;
	}

	PreviewMeshComp->SetStaticMesh(SourceMeshComp->GetStaticMesh());

	const int32 MaterialCount = SourceMeshComp->GetNumMaterials();
	for (int32 i = 0; i < MaterialCount; ++i)
	{
		if (PreviewMaterial)
		{
			PreviewMeshComp->SetMaterial(i, PreviewMaterial);
		}
		else
		{
			PreviewMeshComp->SetMaterial(i, SourceMeshComp->GetMaterial(i));
		}
	}

	PreviewActor->SetActorEnableCollision(false);
	PreviewActor->SetIsTemporarilyHiddenInEditor(false);

	PreviewActor->SetActorLabel(TEXT("PreviewActor"));
	PreviewActor->bIsEditorOnlyActor = true;

	return PreviewActor;
#else
	return nullptr;
#endif
}

void ULevelDesignSupportToolModel::InitializePreviewMaterial()
{
	if (PreviewMaterial)
	{
		return;
	}

	PreviewMaterial = LoadObject<UMaterialInterface>(
		nullptr,
		TEXT("/LevelDesignSupportTool/Materials/M_PreviewActorMaterial.M_PreviewActorMaterial")
	);

	if (!PreviewMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load preview material."));
	}
}
#pragma endregion

#pragma region Preview Apply/Clear
void ULevelDesignSupportToolModel::ApplyPreview(const UTransformOffsetSettings* TransformSettings, const URandomOffsetSettings* RandomSettings)
{
	if (!HasPreviewDatas || PreviewDatas.IsEmpty())
	{
		BuildCombinedPreview(TransformSettings, RandomSettings);
	}

	if (!HasPreviewDatas || PreviewDatas.IsEmpty())
	{
		return;
	}

	const FScopedTransaction Transaction(
		NSLOCTEXT("LevelDesignSupportToolModel", "ApplyPreview", "ApplyPreview")
	);

	for (const FActorPreviewData& Data : PreviewDatas)
	{
		if (!Data.SourceActor) continue;

		Data.SourceActor->Modify();
		Data.SourceActor->SetActorTransform(Data.PreviewTransform);
	}

	ClearPreview();
}

void ULevelDesignSupportToolModel::ClearPreview()
{
#if WITH_EDITOR
	for (FActorPreviewData& Data : PreviewDatas)
	{
		if (Data.PreviewActor)
		{
			Data.PreviewActor->Destroy();
		}
	}

	PreviewDatas.Empty();
	HasPreviewDatas = false;
#endif
}
#pragma endregion