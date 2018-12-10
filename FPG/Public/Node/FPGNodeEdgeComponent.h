// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "FPGNode.h"

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FPGNodeEdgeComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPG_API UFPGNodeEdgeComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UFPGNodeEdgeComponent();

	// connects this edge to the given edge, and sets ConnectedNode
	void SnapToNodeEdge(class UFPGNodeEdgeComponent* OtherNodeEdge);

	// gets the next node in the tree
	FORCEINLINE class AFPGNode* GetConnectedNode() const { return ConnectedNode; }
	FORCEINLINE void ClearConnectedNode()
	{ 
		if (ConnectedNodeEdge)
		{
			ConnectedNodeEdge->ConnectedNode = nullptr;
			ConnectedNodeEdge->ConnectedNodeEdge = nullptr;
		}
		ConnectedNode = nullptr; 
		ConnectedNodeEdge = nullptr;
	}

	// gets our node
	FORCEINLINE class AFPGNode* GetNode() const { return CastChecked<AFPGNode>(this->GetOwner()); }

	FORCEINLINE class UBoxComponent* GetEdgeTrigger() const { return EdgeTrigger;  }
	FORCEINLINE class UArrowComponent* GetEdgeConnector() const { return EdgeConnector; }
	
protected:

	// helper function to connect two of these edge components
	void SnapParentToChildSubComponent(AActor* const Parent, const USceneComponent* const Offset, const FTransform& TowardTransform);

protected:
	// the transform point for all connections of nodes.
	UPROPERTY(Category = Node, EditAnywhere)
	class UArrowComponent* EdgeConnector;

	// the edge trigger will be fired to set this as the new root component
	UPROPERTY(Category = Node, EditAnywhere)
	class UBoxComponent* EdgeTrigger;	
	
	UPROPERTY()
	class AFPGNode* ConnectedNode;

	UPROPERTY()
	UFPGNodeEdgeComponent* ConnectedNodeEdge;
};
