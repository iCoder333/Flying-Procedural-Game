// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPGMainHUD.h"
#include "FPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API AFPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFPGPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void LoadGame();
	void SaveGame();

	void TogglePlaySound(bool IsChecked, bool bSave);
	void ToggleInvertYAxis(bool IsChecked, bool bSave);
	void ToggleInvertXAxis(bool IsChecked, bool bSave);
	void ToggleLeftHanded(bool IsChecked, bool bSave);

	bool IsPlaySound();
	bool IsInvertYAxis();
	bool IsInvertXAxis();
	bool IsLeftHanded();

	void AddCrystals(int32 Num);
	void OnDeath();

protected:
	void MoveHorizontalInput(float Val);
	void MoveVerticalInput(float Val);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFPGMainHUD> MainHUDClass;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* AddCrystalSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* DeathSound;

	UPROPERTY(EditDefaultsOnly)
	class USoundCue* BackgroundMusic;

private:
	UPROPERTY()
	class UFPGSaveGame* LoadGameInstance;

	UPROPERTY()
	class UFPGMainHUD* MainHUD;

	int32 Crystals;
};
