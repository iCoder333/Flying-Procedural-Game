// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPGRandomMaterialComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPG_API UFPGRandomMaterialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPGRandomMaterialComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void RandomizeMeshTextures();

	UPROPERTY(Category = Node, EditAnywhere)
	TArray< class UTexture* > RandomTextures;	
};
