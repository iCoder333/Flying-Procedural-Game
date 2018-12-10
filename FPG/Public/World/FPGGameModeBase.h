// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPGNode.h"
#include "FPGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API AFPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
