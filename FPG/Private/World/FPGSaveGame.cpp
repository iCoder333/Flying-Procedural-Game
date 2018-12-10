// Copyright (c) 2017 - zerosum0x0

#include "FPGSaveGame.h"

UFPGSaveGame::UFPGSaveGame()
{
	SaveSlotName = TEXT("FPG");
	UserIndex = 0;
	bPlaySounds = true;
	bInvertXAxis = false;
	bInvertYAxis = false;
	bLeftHanded = false;
	CrystalHighScore = 0;
}