// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"

template<typename T>
FORCEINLINE static void TArrayShuffle(TArray<T>& TargetArray)
{
	if (TargetArray.Num() == 0)
		return;

	int32 LastIndex = TargetArray.Num() - 1;
	for (int32 i = 0; i < LastIndex; ++i)
	{
		int32 Index = FMath::RandRange(0, LastIndex);
		if (i != Index)
		{
			TargetArray.Swap(i, Index);
		}
	}
}

template<typename T>
FORCEINLINE static T* TArrayRandomChoice(const TArray<T*>& TargetArray)
{
	if (TargetArray.Num() == 0)
		return nullptr;

	return TargetArray[FMath::RandRange(0, TargetArray.Num() - 1)];
}

/** Helper macro for UI, to load a UMG control into the C++ */
/** C++17 templates cannot expand 1:1 to this */
#define FPG_INIT_CONTROL_STR_VAL(arg) #arg
#define FPG_INIT_CONTROL(ControlType, ControlName) \
static const FName ControlName##Name = FName(TEXT(FPG_INIT_CONTROL_STR_VAL(ControlName)));  \
ControlName = ( ControlType *)GetWidgetFromName( ControlName##Name ); \
if (! ControlName ) return false;