// Copyright (c) 2017 - zerosum0x0

#include "FPGPlayerController.h"
#include "FPGPlayerPawn.h"
#include "FPGSaveGame.h"
#include "FPGMainHUD.h"

#include "Engine.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/TouchInterface.h"
#include "Widgets/Input/SVirtualJoystick.h"

AFPGPlayerController::AFPGPlayerController()
{
}

void AFPGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	LoadGame();

	if (MainHUDClass != nullptr)
	{
		MainHUD = CreateWidget<UFPGMainHUD>(this, MainHUDClass);

		if (MainHUD != nullptr)
			MainHUD->AddToViewport();
	}
}

void AFPGPlayerController::LoadGame()
{
	LoadGameInstance = CastChecked<UFPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UFPGSaveGame::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex))
		LoadGameInstance = Cast<UFPGSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	else
		SaveGame();

	ToggleLeftHanded(LoadGameInstance->bLeftHanded, false);
	TogglePlaySound(LoadGameInstance->bPlaySounds, false);
	ToggleInvertYAxis(LoadGameInstance->bInvertYAxis, false);
	ToggleInvertXAxis(LoadGameInstance->bInvertXAxis, false);
}

void AFPGPlayerController::SaveGame()
{
	if (LoadGameInstance)
		UGameplayStatics::SaveGameToSlot(LoadGameInstance, LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex);
}

void AFPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
		return;

	InputComponent->BindAxis("MoveVertical", this, &AFPGPlayerController::MoveVerticalInput);
	InputComponent->BindAxis("MoveHorizontal", this, &AFPGPlayerController::MoveHorizontalInput);
}

void AFPGPlayerController::MoveVerticalInput(float Val)
{
	auto Pawn = Cast<AFPGPlayerPawn>(GetPawn());

	if (Pawn)
	{
		if (LoadGameInstance != nullptr && LoadGameInstance->bInvertYAxis)
			Val = -Val;

		Pawn->MoveAxisVertical(Val);
	}
}

void AFPGPlayerController::MoveHorizontalInput(float Val)
{
	auto Pawn = Cast<AFPGPlayerPawn>(GetPawn());

	if (Pawn)
	{
		if (LoadGameInstance != nullptr && LoadGameInstance->bInvertXAxis)
			Val = -Val;

		Pawn->MoveAxisHorizontal(Val);
	}
}

void AFPGPlayerController::TogglePlaySound(bool IsChecked, bool bSave)
{
	if (GEngine != nullptr)
	{
		FAudioDevice* Audio = GEngine->GetMainAudioDevice();
	
		if (Audio != nullptr)
			Audio->SetDeviceMuted(!IsChecked);
	}

	LoadGameInstance->bPlaySounds = IsChecked;

	if (bSave)
		SaveGame();
}

void AFPGPlayerController::ToggleInvertYAxis(bool IsChecked, bool bSave)
{
	LoadGameInstance->bInvertYAxis = IsChecked;

	if (bSave)
		SaveGame();
}

void AFPGPlayerController::ToggleInvertXAxis(bool IsChecked, bool bSave)
{
	LoadGameInstance->bInvertXAxis = IsChecked;

	if (bSave)
		SaveGame();
}

void AFPGPlayerController::ToggleLeftHanded(bool IsChecked, bool bSave)
{
	LoadGameInstance->bLeftHanded = IsChecked;

	if (CurrentTouchInterface != nullptr && CurrentTouchInterface->Controls.Num() > 0)
	{
		FVector2D NewPlacement = (IsChecked) ? FVector2D(135.f, -135.f) : FVector2D(-135.f, -135.f);
		CurrentTouchInterface->Controls[0].Center = NewPlacement;
		ActivateTouchInterface(this->CurrentTouchInterface);
	}

	if (bSave)
		SaveGame();
}


void AFPGPlayerController::AddCrystals(int32 Num)
{
	Crystals += Num;

	if (Crystals > LoadGameInstance->CrystalHighScore)
	{
		LoadGameInstance->CrystalHighScore = Crystals;
		SaveGame();

		if (MainHUD != nullptr)
			MainHUD->ShowTrophy();
	}
	
	if (AddCrystalSound != nullptr)
		UGameplayStatics::PlaySound2D(GetWorld(), AddCrystalSound);
	
	if (MainHUD != nullptr)
		MainHUD->SetCrystalCount(Crystals);
}


void AFPGPlayerController::OnDeath()
{
	if (MainHUD != nullptr)
		MainHUD->ShowDeathMenu(LoadGameInstance->CrystalHighScore, Crystals);
}

bool AFPGPlayerController::IsPlaySound()
{
	return LoadGameInstance->bPlaySounds;
}

bool AFPGPlayerController::IsInvertYAxis()
{
	return LoadGameInstance->bInvertYAxis;
}

bool AFPGPlayerController::IsInvertXAxis()
{
	return LoadGameInstance->bInvertXAxis;
}

bool AFPGPlayerController::IsLeftHanded()
{
	return LoadGameInstance->bLeftHanded;
}




