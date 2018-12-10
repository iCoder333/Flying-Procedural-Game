// Copyright (c) 2017 - zerosum0x0

#include "FPGDeathMenu.h"
#include "FPG.h"
#include "UMG.h"
#include "Kismet/KismetSystemLibrary.h"

bool UFPGDeathMenu::Initialize()
{
	if (!Super::Initialize())
		return false;
	
	FPG_INIT_CONTROL(UTextBlock, TextBlockCrystals);
	FPG_INIT_CONTROL(UTextBlock, TextBlockCrystalsHigh);
	FPG_INIT_CONTROL(UButton, ButtonReload);

	ButtonReload->OnClicked.AddDynamic(this, &UFPGDeathMenu::OnClickedButtonReload);

	const int adno = 0; // readl
	//const int adno = 1; // test
	UKismetSystemLibrary::LoadInterstitialAd(adno);

	return true;
}

void UFPGDeathMenu::SetScores(int32 CrystalsHigh, int32 Crystals)
{
	TextBlockCrystals->SetText(FText::FromString(FString::FromInt(Crystals)));
	TextBlockCrystalsHigh->SetText(FText::FromString(FString::FromInt(CrystalsHigh)));
}

void UFPGDeathMenu::OnClickedButtonReload()
{
	UKismetSystemLibrary::ShowInterstitialAd();

	auto PlayerController = GetOwningPlayer();

	if (PlayerController != nullptr)
	{
		PlayerController->RestartLevel();
	}
}


