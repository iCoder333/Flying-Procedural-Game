// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"

#include "FPGTrap.h"
#include "FPGCannonProjectile.h"

#include "FPGCannonTrap.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API AFPGCannonTrap : public AFPGTrap
{
	GENERATED_BODY()

public:
	AFPGCannonTrap();

	UFUNCTION()
	void FireCannon();

protected:
	virtual void BeginPlay() override;
	
	
protected:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditDefaultsOnly)
	class UArrowComponent* Muzzle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFPGCannonProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float FireDelay;

	FTimerHandle TimerHandle_FireDelay;
};
