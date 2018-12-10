// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Traps/FPGTrap.h"
#include "FPGGuillotineTrap.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FPG_API AFPGGuillotineTrap : public AFPGTrap
{
	GENERATED_BODY()

public:
	AFPGGuillotineTrap();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

	virtual void StartTrap() override;
	virtual void StopTrap() override;

	UFUNCTION()
	void MoveBladeUp();
	
	UFUNCTION()
	void MoveBladeDown();

	UFUNCTION()
	void BladeTimelineUpdateCallback(float UpdateValue);

	UFUNCTION()
	void BladeTimelineFinishCallback();

protected:

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	class USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(Category = Traps, EditDefaultsOnly)
	class UStaticMeshComponent* BladeMesh;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	class UStaticMeshComponent* FrameMesh;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	UCurveFloat* PlayCurve;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float DownPlayRate;
	
	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float UpPlayRate;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float LoopDelayRate;

private:
	UPROPERTY()
	class UTimelineComponent* BladeTimeline;

	FOnTimelineFloat BladeTimelineInterpFunction{};
	FOnTimelineEvent BladeTimelineFinishFunction{};

	FTimerHandle LoopTimerHandle;

	FVector StartBladeLocation;

	enum class BladeDirection
	{
		Up,
		Down
	};

	BladeDirection CurrentDirection;
};
