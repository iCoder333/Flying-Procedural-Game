// Copyright (c) 2017 - zerosum0x0

#include "FPGSpawnGroup.h"

#include "FPG.h"
#include "FPGTrap.h"

#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"


UFPGSpawnGroup::UFPGSpawnGroup()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFPGSpawnGroup::BeginPlay()
{
	Super::BeginPlay();
}
