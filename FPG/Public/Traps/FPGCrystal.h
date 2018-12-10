// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Traps/FPGTrap.h"
#include "FPGCrystal.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API AFPGCrystal : public AFPGTrap
{
	GENERATED_BODY()

public:
	AFPGCrystal();

	UFUNCTION()
	void OnCrystalBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* Capsule;	
};
