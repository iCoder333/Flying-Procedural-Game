// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FPGSettingsMenu.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API UFPGSettingsMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	UFUNCTION()
	void OnCheckBoxPlaySound(bool IsChecked);

	UFUNCTION()
	void OnCheckBoxInvertYAxis(bool IsChecked);

	UFUNCTION()
	void OnCheckBoxInvertXAxis(bool IsChecked);

	UFUNCTION()
	void OnCheckBoxLeftHanded(bool IsChecked);

	UFUNCTION()
	void OnClickedButtonDone();

private:
	UPROPERTY()
	class AFPGPlayerController* FPGPlayerController;

	class UCheckBox* CheckBoxPlaySound;
	class UCheckBox* CheckBoxInvertYAxis;
	class UCheckBox* CheckBoxInvertXAxis;
	class UCheckBox* CheckBoxLeftHanded;

	class UButton* ButtonDone;
};
