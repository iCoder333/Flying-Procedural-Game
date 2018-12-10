// Copyright (c) 2017 - zerosum0x0

#include "FPGGuillotineTrap.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

AFPGGuillotineTrap::AFPGGuillotineTrap()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	FrameMesh->SetupAttachment(RootComponent);

	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeMesh"));
	BladeMesh->SetupAttachment(RootComponent);

	BladeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BladeTimeline"));
	BladeTimeline->SetLooping(false);

	BladeTimelineInterpFunction.BindDynamic(this, &AFPGGuillotineTrap::BladeTimelineUpdateCallback);
	BladeTimelineFinishFunction.BindDynamic(this, &AFPGGuillotineTrap::BladeTimelineFinishCallback);

	DownPlayRate = 1.f;
	UpPlayRate = 0.3f;
	LoopDelayRate = 1.f;
}

void AFPGGuillotineTrap::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!PlayCurve)
	{
		PlayCurve = NewObject<UCurveFloat>(this, TEXT("PlayCurve"));
	}

	StartBladeLocation = BladeMesh->RelativeLocation;

	FVector BladeMin, BladeMax, FrameMin, FrameMax;
	BladeMesh->GetLocalBounds(BladeMin, BladeMax);
	FrameMesh->GetLocalBounds(FrameMin, FrameMax);
	StartBladeLocation.Z = BladeMin.Z + FrameMax.Z;

	BladeMesh->SetRelativeLocation(StartBladeLocation, false);
}

void AFPGGuillotineTrap::BeginPlay()
{
	Super::BeginPlay();

	BladeTimeline->AddInterpFloat(PlayCurve, BladeTimelineInterpFunction);
	BladeTimeline->SetTimelineFinishedFunc(BladeTimelineFinishFunction);

	MoveBladeDown();
}

void AFPGGuillotineTrap::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(LoopTimerHandle);
}

void AFPGGuillotineTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPGGuillotineTrap::MoveBladeUp()
{
	CurrentDirection = BladeDirection::Up;
	BladeTimeline->SetPlayRate(UpPlayRate);
	BladeTimeline->Reverse();
}

void AFPGGuillotineTrap::MoveBladeDown()
{
	CurrentDirection = BladeDirection::Down;
	BladeTimeline->SetPlayRate(DownPlayRate);
	BladeTimeline->Play();
}

void AFPGGuillotineTrap::BladeTimelineUpdateCallback(float UpdateValue)
{
	FVector NewLocation(BladeMesh->RelativeLocation.X, BladeMesh->RelativeLocation.Y, StartBladeLocation.Z * UpdateValue);
	BladeMesh->SetRelativeLocation(NewLocation, true);
}

void AFPGGuillotineTrap::BladeTimelineFinishCallback()
{
	if (CurrentDirection == BladeDirection::Up)
	{
		// wait a sec before dropping again...
		GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &AFPGGuillotineTrap::MoveBladeDown, LoopDelayRate, false);
	}
	else
	{
		MoveBladeUp();
	}
}

void AFPGGuillotineTrap::StartTrap()
{
	if (CurrentDirection == BladeDirection::Up)
	{
		MoveBladeUp();
	}
	else
	{
		MoveBladeDown();
	}
}

void AFPGGuillotineTrap::StopTrap()
{
	BladeTimeline->Stop();
}
