// Copyright (c) 2017 - zerosum0x0

//////////////////////////////
/* WARNING: HERE BE DRAGONS */
//////////////////////////////

#pragma once

#include "FPGNode.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FPGNodeManager.generated.h"

/**
* This actor is placed in a level to spawn and despawn nodes.
* This was originally done through recursion, but was rewritten.
* This definitely can and should be optimized......
**/
UCLASS(abstract)
class FPG_API AFPGNodeManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPGNodeManager();

	UFUNCTION()
	void OnEdgeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

protected:
	/* What class the origin node at level start should be */
	UPROPERTY(Category = Node, EditDefaultsOnly)
	TSubclassOf<AFPGNode> StartNodeType;

	/* How far to recurse for creating and destroying nodes */
	UPROPERTY(Category = Node, EditDefaultsOnly)
	int32 NodeDepth;

protected:
	UPROPERTY(Category = Node, EditDefaultsOnly)
	TArray< TSubclassOf<AFPGNode> > NodeTypes;

private:
	class AFPGNode* SpawnNode(UClass* NodeClass);
	class AFPGNode* SpawnRandomNode();

	void SetRootNode(class AFPGNode* NewRootNode);

	// returns true if this Node is placed in a non-blocking location.
	bool CheckNodeBounds(class AFPGNode* Node, class AFPGNode* SpawningNode);

	TArray<class AFPGNode*> GetOffendingBounds(class AFPGNode* Node, class AFPGNode* SpawningNode);

	UPROPERTY()
	class AFPGNode* RootNode;

	UPROPERTY()
	TArray<AFPGNode*> NextNodeList;

	UPROPERTY()
	TArray<AFPGNode*> VisitedNodeList;
};
