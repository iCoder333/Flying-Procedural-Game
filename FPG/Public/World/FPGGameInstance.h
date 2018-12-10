// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API UFPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& MapName);
	
	UFUNCTION()
	virtual void EndLoadingScreen(class UWorld *World);
	
};
