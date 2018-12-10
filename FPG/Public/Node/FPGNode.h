// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPGTrap.h"
#include "FPGNode.generated.h"


UCLASS()
class FPG_API AFPGNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPGNode();

	TArray<class UFPGNodeEdgeComponent*> GetNodeEdges();

	FORCEINLINE FBox GetNodeBox() const { return Mesh->Bounds.GetBox(); }

	void SpawnTraps();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* DefaultSceneRoot;

private:
	UPROPERTY()
	TArray<AFPGTrap*> SpawnedTraps;
};
