// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FPGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API UFPGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UFPGSaveGame();
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	bool bPlaySounds;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	bool bInvertYAxis;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	bool bInvertXAxis;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	bool bLeftHanded;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	int32 CrystalHighScore;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Basic)
	int32 UserIndex;
};
