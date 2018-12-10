// Copyright (c) 2017 - zerosum0x0

#include "FPGSettingsMenu.h"

#include "FPG.h"
#include "FPGPlayerController.h"

#include "UMG.h"

bool UFPGSettingsMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	FPGPlayerController = Cast<AFPGPlayerController>(GetOwningPlayer());

	if (FPGPlayerController == nullptr)
		return false;

	FPG_INIT_CONTROL(UCheckBox, CheckBoxPlaySound);
	FPG_INIT_CONTROL(UCheckBox, CheckBoxInvertYAxis);
	FPG_INIT_CONTROL(UCheckBox, CheckBoxInvertXAxis);
	FPG_INIT_CONTROL(UCheckBox, CheckBoxLeftHanded);
	FPG_INIT_CONTROL(UButton, ButtonDone);

	CheckBoxPlaySound->SetIsChecked(FPGPlayerController->IsPlaySound());
	CheckBoxInvertYAxis->SetIsChecked(FPGPlayerController->IsInvertYAxis());
	CheckBoxInvertXAxis->SetIsChecked(FPGPlayerController->IsInvertXAxis());
	CheckBoxLeftHanded->SetIsChecked(FPGPlayerController->IsLeftHanded());

	CheckBoxPlaySound->OnCheckStateChanged.AddDynamic(this, &UFPGSettingsMenu::OnCheckBoxPlaySound);
	CheckBoxInvertYAxis->OnCheckStateChanged.AddDynamic(this, &UFPGSettingsMenu::OnCheckBoxInvertYAxis);
	CheckBoxInvertXAxis->OnCheckStateChanged.AddDynamic(this, &UFPGSettingsMenu::OnCheckBoxInvertXAxis);
	CheckBoxLeftHanded->OnCheckStateChanged.AddDynamic(this, &UFPGSettingsMenu::OnCheckBoxLeftHanded);

	ButtonDone->OnClicked.AddDynamic(this, &UFPGSettingsMenu::OnClickedButtonDone);

	return true;
}

void UFPGSettingsMenu::OnClickedButtonDone()
{
	SetVisibility(ESlateVisibility::Hidden);
	GetOwningPlayer()->SetPause(false);
}

void UFPGSettingsMenu::OnCheckBoxPlaySound(bool IsChecked)
{
	FPGPlayerController->TogglePlaySound(IsChecked, true);
}

void UFPGSettingsMenu::OnCheckBoxInvertYAxis(bool IsChecked)
{
	FPGPlayerController->ToggleInvertYAxis(IsChecked, true);
}

void UFPGSettingsMenu::OnCheckBoxInvertXAxis(bool IsChecked)
{
	FPGPlayerController->ToggleInvertXAxis(IsChecked, true);
}

void UFPGSettingsMenu::OnCheckBoxLeftHanded(bool IsChecked)
{
	FPGPlayerController->ToggleLeftHanded(IsChecked, true);
}

