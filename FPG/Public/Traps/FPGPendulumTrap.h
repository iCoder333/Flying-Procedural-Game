// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Traps/FPGTrap.h"
#include "FPGPendulumTrap.generated.h"

/**
 * 
 */
UCLASS()
class FPG_API AFPGPendulumTrap : public AFPGTrap
{
	GENERATED_BODY()
public:
	AFPGPendulumTrap();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void PendulumTimelineUpdateCallback(float UpdateValue);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* WirePivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* BobPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInstancedStaticMeshComponent* WireMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* BobMesh;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	class UCurveFloat* PolarCurve;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float PolarAngle;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float WireLength;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float PlayRate;

private:
	FORCEINLINE void UpdatePolarTransforms(float NewPolar);
	void MakeWireInstances();

	UPROPERTY()
	class UTimelineComponent* PendulumTimeline;

	FOnTimelineFloat PendulumTimelineInterpFunction{};
};
