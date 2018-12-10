// Copyright (c) 2017 - zerosum0x0

#include "FPGCrystal.h"

#include "FPGPlayerPawn.h"
#include "FPGPlayerController.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"

AFPGCrystal::AFPGCrystal()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);

	Mesh->bGenerateOverlapEvents = false;
	
	Capsule->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AFPGCrystal::OnCrystalBeginOverlap);
}

void AFPGCrystal::OnCrystalBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Pawn = Cast<AFPGPlayerPawn>(OtherActor);
	if (Pawn)
	{
		auto PlayerController = Cast<AFPGPlayerController>(Pawn->GetController());

		if (PlayerController)
		{
			PlayerController->AddCrystals(1);
			this->Destroy();
		}
	}

}

