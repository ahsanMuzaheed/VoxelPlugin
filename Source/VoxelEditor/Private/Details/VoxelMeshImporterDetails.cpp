// Copyright 2020 Phyronnaz

#include "VoxelMeshImporterDetails.h"
#include "VoxelImporters/VoxelMeshImporter.h"

#include "Factories/VoxelDataAssetFactory.h"
#include "VoxelAssets/VoxelDataAsset.h"

#include "VoxelEditorDetailsUtilities.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "Voxel"

TSharedRef<IDetailCustomization> FVoxelMeshImporterDetails::MakeInstance()
{
	return MakeShareable(new FVoxelMeshImporterDetails());
}

void FVoxelMeshImporterDetails::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	FVoxelEditorUtilities::EnableRealtime();
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailLayout.GetObjectsBeingCustomized(Objects);
	if (Objects.Num() != 1)
	{
		return;
	}
	MeshImporter = CastChecked<AVoxelMeshImporter>(Objects[0].Get());;

	FVoxelEditorUtilities::AddButtonToCategory(DetailLayout,
		"Create VoxelDataAsset from Mesh",
		LOCTEXT("Create", "Create"),
		LOCTEXT("CreateFromMesh", "Create From Mesh"),
		LOCTEXT("Create", "Create"),
		false,
		FOnClicked::CreateSP(this, &FVoxelMeshImporterDetails::OnCreateFromMesh),
		TAttribute<bool>::Create(TAttribute<bool>::FGetter::CreateLambda([Importer = MeshImporter]() { return Importer.IsValid() && Importer->StaticMesh; })));
}

FReply FVoxelMeshImporterDetails::OnCreateFromMesh()
{
	auto* Factory = NewObject<UVoxelDataAssetFromMeshImporterFactory>();
	Factory->MeshImporter = MeshImporter.Get();

	FVoxelEditorUtilities::CreateAssetWithDialog(UVoxelDataAsset::StaticClass(), Factory);

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE