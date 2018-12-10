// Copyright (c) 2017 - zerosum0x0

#include "FPGGameInstance.h"

#include "Engine/World.h"
#include "MoviePlayer.h"

void UFPGGameInstance::Init()
{
	UGameInstance::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UFPGGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UFPGGameInstance::EndLoadingScreen);
}

void UFPGGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 1.f;
		LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UFPGGameInstance::EndLoadingScreen(UWorld* World)
{

}


