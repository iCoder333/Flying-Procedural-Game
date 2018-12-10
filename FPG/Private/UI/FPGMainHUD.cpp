// Copyright (c) 2017 - zerosum0x0

#include "FPGMainHUD.h"
#include "FPG.h"
#include "UMG.h"

bool UFPGMainHUD::Initialize()
{
	if (!Super::Initialize())
		return false;

	FPG_INIT_CONTROL(UTextBlock, TextBlockCrystals);
	FPG_INIT_CONTROL(UButton, ButtonSettings);
	FPG_INIT_CONTROL(UImage, ImageTrophy);

	ButtonSettings->OnClicked.AddDynamic(this, &UFPGMainHUD::ShowSettingsMenu);

	DeathMenu = CreateMenu<UFPGDeathMenu>(DeathMenuClass);
	SettingsMenu = CreateMenu<UFPGSettingsMenu>(SettingsMenuClass);
	/*
	SettingsMenu = CreateWidget<UFPGSettingsMenu>(GetOwningPlayer(), SettingsMenuClass);

	if (SettingsMenu != nullptr)
	{
		SettingsMenu->SetVisibility(ESlateVisibility::Hidden);
		SettingsMenu->AddToViewport();
	}

	DeathMenu = CreateWidget<UFPGDeathMenu>(GetOwningPlayer(), DeathMenuClass);

	if (DeathMenu != nullptr)
	{
		DeathMenu->SetVisibility(ESlateVisibility::Hidden);
		DeathMenu->AddToViewport();
	}
	*/
	return true;
}


template <typename T>
T* UFPGMainHUD::CreateMenu(UClass* MenuClass)
{
	T* Menu = CreateWidget<T>(GetOwningPlayer(), MenuClass);

	if (Menu != nullptr)
	{
		Menu->SetVisibility(ESlateVisibility::Hidden);
		Menu->AddToViewport();
	}

	return Menu;
}

void UFPGMainHUD::ShowSettingsMenu()
{
	if (SettingsMenu != nullptr)
	{
		if (SettingsMenu->GetVisibility() == ESlateVisibility::Visible)
		{
			SettingsMenu->OnClickedButtonDone();
		}
		else
		{
			GetOwningPlayer()->SetPause(true);
			SettingsMenu->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UFPGMainHUD::ShowDeathMenu(int32 CrystalsHigh, int32 Crystals)
{
	if (DeathMenu != nullptr)
	{
		DeathMenu->SetScores(CrystalsHigh, Crystals);
		DeathMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

void UFPGMainHUD::SetCrystalCount(int32 Count)
{
	TextBlockCrystals->SetText(FText::FromString(FString::FromInt(Count)));
}

void UFPGMainHUD::ShowTrophy()
{
	ImageTrophy->SetVisibility(ESlateVisibility::Visible);
}
