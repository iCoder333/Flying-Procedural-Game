// Copyright (c) 2017 - zerosum0x0

#include "FPGNode.h"

#include "FPG.h"
#include "FPGNodeEdgeComponent.h"
#include "FPGSpawnGroup.h"

#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPGNode::AFPGNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetActorEnableCollision(true);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->BodyInstance.bUseCCD = false; 
}

// Called when the game starts or when spawned
void AFPGNode::BeginPlay()
{
	Super::BeginPlay();
}

TArray<UFPGNodeEdgeComponent*> AFPGNode::GetNodeEdges()
{
	TArray<UFPGNodeEdgeComponent*> NodeEdges;
	GetComponents<UFPGNodeEdgeComponent>(NodeEdges);

	return NodeEdges;
}

void AFPGNode::SpawnTraps()
{
	TArray<UFPGSpawnGroup*> SpawnGroups;
	GetComponents<UFPGSpawnGroup>(SpawnGroups);

	for (auto &SpawnGroup : SpawnGroups)
	{
		AFPGTrap* NewTrap = SpawnGroup->Spawn<AFPGTrap>();

		//CreateDefaultSubobject<UChildActorComponent>()
		if (NewTrap != nullptr)
		{
			//this->Children.Add(NewTrap);
			SpawnedTraps.Add(NewTrap);
		}
	}
}

void AFPGNode::EndPlay(const EEndPlayReason::Type Reason)
{
	// despawn traps on a random delay to improve framerate
	const float LifeSpan = FMath::RandRange(0.01f, 0.5f);

	for (auto &Trap : SpawnedTraps)
	{
		if (Trap != nullptr && Trap->IsValidLowLevel())
		{
			//Trap->Destroy();
			Trap->SetLifeSpan(LifeSpan);
		}
	}

	SpawnedTraps.Empty();
}


