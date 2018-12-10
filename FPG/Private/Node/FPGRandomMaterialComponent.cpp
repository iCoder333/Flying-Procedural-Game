// Copyright (c) 2017 - zerosum0x0

#include "FPGRandomMaterialComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "FPG.h"

UFPGRandomMaterialComponent::UFPGRandomMaterialComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFPGRandomMaterialComponent::BeginPlay()
{
	Super::BeginPlay();

	RandomizeMeshTextures();
}

void UFPGRandomMaterialComponent::RandomizeMeshTextures()
{
	
	if (RandomTextures.Num() < 2 || !GetOwner())
		return;

	TArrayShuffle(RandomTextures);

	TArray<UStaticMeshComponent *> Components;
	GetOwner()->GetComponents<UStaticMeshComponent>(Components);

	for (auto &StaticMeshComponent : Components)
	{
		for (int32 i = 0; i < StaticMeshComponent->GetNumMaterials(); ++i)
		{
			UMaterialInterface* CurrentMat = StaticMeshComponent->GetMaterial(i);
			UMaterialInstanceDynamic* RandMat = UMaterialInstanceDynamic::Create(CurrentMat, StaticMeshComponent);

			UTexture* RandWall = RandomTextures[0];
			UTexture* RandFloor = RandomTextures[1];

			RandMat->SetTextureParameterValue(FName(TEXT("WallTexture")), RandWall);
			RandMat->SetTextureParameterValue(FName(TEXT("FloorTexture")), RandFloor);

			StaticMeshComponent->SetMaterial(i, RandMat);
		}
	}
	/**/
}

