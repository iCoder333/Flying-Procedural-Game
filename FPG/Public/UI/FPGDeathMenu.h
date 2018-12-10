// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPGDeathMenu.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API UFPGDeathMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetScores(int32 CrystalsHigh, int32 Crystals);

	UFUNCTION()
	void OnClickedButtonReload();
	
private:
	class UTextBlock* TextBlockCrystalsHigh;
	class UTextBlock* TextBlockCrystals;	
	class UButton* ButtonReload;
};
