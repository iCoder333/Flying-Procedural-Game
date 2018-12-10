// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPGTrap.generated.h"

UCLASS()
class FPG_API AFPGTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPGTrap();

	// we'd go pure virtual, but some traps may be static?
	virtual void StartTrap() {};
	virtual void StopTrap() {};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
