// Copyright (c) 2017 - zerosum0x0

#include "FPGCannonTrap.h"

#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

AFPGCannonTrap::AFPGCannonTrap()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<UArrowComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);

	FireDelay = 2.f;
}

void AFPGCannonTrap::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_FireDelay, this, &AFPGCannonTrap::FireCannon, FireDelay, true, FireDelay);
}

void AFPGCannonTrap::FireCannon()
{
	FTransform MuzzleTransform = Muzzle->GetComponentTransform();
	const FVector MuzzleForwardVector = Muzzle->GetForwardVector();

	MuzzleTransform.SetLocation(MuzzleTransform.GetLocation() + MuzzleForwardVector);

	GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleTransform);
}

