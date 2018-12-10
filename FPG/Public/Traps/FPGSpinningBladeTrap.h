// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Traps/FPGTrap.h"
#include "FPGSpinningBladeTrap.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPG_API AFPGSpinningBladeTrap : public AFPGTrap
{
	GENERATED_BODY()
	
public:
	AFPGSpinningBladeTrap();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//virtual void StartTrap() override;
	//virtual void StopTrap() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInstancedStaticMeshComponent* BladeMesh;
	
	UPROPERTY(Category = Traps, EditDefaultsOnly)
	int32 BladeCount;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float RevolutionRate;
	
	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float DelayRate;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	UCurveFloat* PlayCurve;
};
