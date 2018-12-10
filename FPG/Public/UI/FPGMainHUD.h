// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "FPGSettingsMenu.h"
#include "FPGDeathMenu.h"

#include "FPGMainHUD.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API UFPGMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize() override;

	UFUNCTION()
	void ShowSettingsMenu();
	
	void ShowDeathMenu(int32 CrystalsHigh, int32 Crystals);
	
	void SetCrystalCount(int32 Crystals);

	void ShowTrophy();
	
protected:
	template <typename T>
	T* CreateMenu(UClass* MenuClass);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFPGSettingsMenu> SettingsMenuClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFPGDeathMenu> DeathMenuClass;

private:
	class UTextBlock* TextBlockCrystals;
	class UButton* ButtonSettings;
	class UImage* ImageTrophy;

	UPROPERTY()
	class UFPGSettingsMenu* SettingsMenu;

	UPROPERTY()
	class UFPGDeathMenu* DeathMenu;
};
