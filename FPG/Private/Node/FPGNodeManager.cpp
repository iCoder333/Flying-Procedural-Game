// Copyright (c) 2017 - zerosum0x0

//////////////////////////////
/* WARNING: HERE BE DRAGONS */
//////////////////////////////

#include "FPGNodeManager.h"

#include "FPG.h"
#include "FPGNode.h"
#include "FPGNodeEdgeComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"

AFPGNodeManager::AFPGNodeManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AFPGNodeManager::BeginPlay()
{
	Super::BeginPlay();
	
	AFPGNode* FirstNode = SpawnNode(StartNodeType);
	
	FirstNode->SpawnTraps();
	SetRootNode(FirstNode);
}

class AFPGNode* AFPGNodeManager::SpawnNode(UClass* NodeClass)
{
	if (NodeClass == nullptr)
		return nullptr;

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//SpawnParameters.

	FTransform NewLoc;

	// spawn far away from player so we don't get an overlap trigger around 0,0,0
	if (RootNode != nullptr)
	{
		static int Offset = 99999;
		FVector RootLoc = RootNode->GetActorLocation();
		NewLoc.SetLocation(FVector(RootLoc.X - Offset, RootLoc.Y - Offset, RootLoc.Z - Offset));
	}

	// Transform does not matter because we spawn and relocate before a render pass
	AFPGNode* Node = GetWorld()->SpawnActor<AFPGNode>(NodeClass, NewLoc, SpawnParameters);

	for (auto &NodeEdge : Node->GetNodeEdges())
		NodeEdge->GetEdgeTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AFPGNodeManager::OnEdgeTriggerBeginOverlap);

	return Node;
}

AFPGNode* AFPGNodeManager::SpawnRandomNode()
{
	if (NodeTypes.Num() == 0)
		return nullptr;

	UClass* NodeClass = NodeTypes[FMath::RandRange(0, NodeTypes.Num() - 1)];

	return SpawnNode(NodeClass);
}

// further optimized to depth = 1 mobile game
void AFPGNodeManager::SetRootNode(class AFPGNode* NewRootNode)
{
	// no need to refresh if this is already the current root
	if (NewRootNode == nullptr || RootNode == NewRootNode)
		return;

	RootNode = NewRootNode;

fuqqit:
	// destroy all old nodes
	for (TActorIterator<AFPGNode> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AFPGNode* OldNode = *ActorItr;

		for (auto &OldNodeEdge : OldNode->GetNodeEdges())
			OldNodeEdge->ClearConnectedNode();

		if (OldNode != RootNode)
		{
			OldNode->Destroy();
		}
	}

	//GetWorld()->ForceGarbageCollection(true);

	int32 Depth = NodeDepth;

	// walk nodes
	auto Node = RootNode;

	for (auto &NodeEdge : Node->GetNodeEdges())
	{
		AFPGNode* NextNode = NodeEdge->GetConnectedNode();

		// component does not have a connected node, let's spawn one.
		if (NextNode == nullptr)
		{
			TArrayShuffle(NodeTypes);

			bool EdgeFound = false;
			for (auto &NodeType : NodeTypes)
			{
				NextNode = SpawnNode(NodeType);

				auto NextNodeEdges = NextNode->GetNodeEdges();
				TArrayShuffle(NextNodeEdges);

				for (auto &NextNodeEdge : NextNodeEdges)
				{
					NextNodeEdge->SnapToNodeEdge(NodeEdge);

					if (CheckNodeBounds(NextNode, Node))
					{
						EdgeFound = true;
						break;
					}

					NodeEdge->ClearConnectedNode();
					//NextNodeEdge->ClearConnectedNode();
				}

				if (EdgeFound)
					break;

				NextNode->Destroy();
				NextNode = nullptr;
			}

			if (NextNode != nullptr)
				NextNode->SpawnTraps();

		}

		// root node always has to have a connection or player goes to outer space
		// On exit from a scope (however accomplished), destructors (12.4) are called for all constructed objects 
		// with automatic storage duration (3.7.2) (named objects or temporaries) that are declared in that scope, 
		// in the reverse order of their declaration.  (i.e. CurrentNodeList will be purged).
		if (NextNode == nullptr && Node == RootNode)
		{
			//CurrentNodeList.Reset(); // I trust the standard, but not the compiler
			goto fuqqit;
		}
	}

}

/*
void AFPGNodeManager::SetRootNode(class AFPGNode* NewRootNode)
{
	// no need to refresh if this is already the current root
	if (NewRootNode == nullptr || RootNode == NewRootNode)
		return;

	RootNode = NewRootNode;

fuqqit:

	NextNodeList.Reset();
	VisitedNodeList.Reset();

	// destroy all old nodes
	for (TActorIterator<AFPGNode> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AFPGNode* OldNode = *ActorItr;
		
		for (auto &OldNodeEdge : OldNode->GetNodeEdges())
			OldNodeEdge->ClearConnectedNode();

		if (OldNode != RootNode)
		{
			OldNode->Destroy();
			//OldNode->SetLifeSpan(0.1f);
		}
	}

	//GetWorld()->ForceGarbageCollection(true);

	int32 Depth = NodeDepth;

	NextNodeList.Push(RootNode);

	while (NextNodeList.Num() > 0 && Depth > 0)
	{
		TArray<AFPGNode*> CurrentNodeList(NextNodeList);
		NextNodeList.Reset();

		// walk nodes
		for (auto &Node : CurrentNodeList)
		{
			VisitedNodeList.Push(Node);

			for (auto &NodeEdge : Node->GetNodeEdges())
			{
				AFPGNode* NextNode = NodeEdge->GetConnectedNode();

				// component does not have a connected node, let's spawn one.
				if (NextNode == nullptr)
				{
					TArrayShuffle(NodeTypes);

					bool EdgeFound = false;
					for (auto &NodeType : NodeTypes)
					{
						NextNode = SpawnNode(NodeType);

						auto NextNodeEdges = NextNode->GetNodeEdges();
						TArrayShuffle(NextNodeEdges);

						for (auto &NextNodeEdge : NextNodeEdges)
						{
							NextNodeEdge->SnapToNodeEdge(NodeEdge);

							if (CheckNodeBounds(NextNode, Node))
							{
								EdgeFound = true;
								break;
							}

							NodeEdge->ClearConnectedNode();
							//NextNodeEdge->ClearConnectedNode();
						}

						if (EdgeFound)
							break;

						NextNode->Destroy();
						NextNode = nullptr;
					}

					if (NextNode != nullptr)
						NextNode->SpawnTraps();

				}

				// root node always has to have a connection or player goes to outer space
				// On exit from a scope (however accomplished), destructors (12.4) are called for all constructed objects 
				// with automatic storage duration (3.7.2) (named objects or temporaries) that are declared in that scope, 
				// in the reverse order of their declaration.  (i.e. CurrentNodeList will be purged).
				if (NextNode == nullptr && Node == RootNode)
				{
					CurrentNodeList.Reset(); // I trust the standard, but not the compiler
					goto fuqqit;
				}

				// eh, we don't care if its root node
				if (NextNode == nullptr)
					continue;

				if (VisitedNodeList.Find(NextNode) != INDEX_NONE)
					continue;

				NextNodeList.Push(NextNode);
			}

		}

		CurrentNodeList.Reset();
		--Depth;
	}

	// purge references for stupid Unreal garbage collector 
	NextNodeList.Reset();
	VisitedNodeList.Reset();
}

*/

/*

// this will cause a crash in approximately 1:2^(2^14)
for (auto &NodeEdge : NewRootNode->GetNodeEdges())
{
if (NodeEdge->GetConnectedNode() == nullptr)
{
RootNode = nullptr;
SetRootNode(NewRootNode);
break;
}
}

*/

/*
void AFPGNodeManager::SetRootNode(class AFPGNode* NewRootNode)
{
	// no need to refresh if this is already the current root
	if (NewRootNode == nullptr || RootNode == NewRootNode)
		return;

	RootNode = NewRootNode;

	NextNodeList.Push(RootNode);

	int32 Depth = NodeDepth;
	// walk depth
	while (NextNodeList.Num() > 0)
	{
		TArray<AFPGNode*> CurrentNodeList(NextNodeList);
		NextNodeList.Reset();

		// walk nodes
		for (auto &Node : CurrentNodeList)
		{
			VisitedNodeList.Push(Node);

			for (auto &NodeEdge : Node->GetNodeEdges())
			{
				AFPGNode* NextNode = NodeEdge->GetConnectedNode();

				// component does not have a connected node, let's spawn one.
				if (!NextNode && Depth > 0)
				{
					TArrayShuffle(NodeTypes);

					bool EdgeFound = false;
					for (auto &NodeType : NodeTypes)
					{
						NextNode = SpawnNode(NodeType);

						auto NextNodeEdges = NextNode->GetNodeEdges();
						TArrayShuffle(NextNodeEdges);

						for (auto &NextNodeEdge : NextNodeEdges)
						{
							NextNodeEdge->SnapToNodeEdge(NodeEdge);

							if (CheckNodeBounds(NextNode, Node))
							{
								EdgeFound = true;
								break;
							}

							NodeEdge->ClearConnectedNode();
							NextNodeEdge->ClearConnectedNode();
						}

						if (EdgeFound)
							break;
						
						NextNode->Destroy();
						NextNode = nullptr;
					}

					// if we're the root node, we'll despawn ppl getting in our way
					// this is some mega dragons
					if (NextNode == nullptr && Node == RootNode)
					{
						NextNode = SpawnNode(NodeTypes[0]);
						NextNode->GetNodeEdges()[0]->SnapToNodeEdge(NodeEdge);

						for (auto &Offender : GetOffendingBounds(NextNode, Node))
						{
							for (auto &OffenderEdge : Offender->GetNodeEdges())
							{
								if (OffenderEdge->GetConnectedNode() != nullptr && OffenderEdge->GetConnectedNode() != RootNode)
								{
									for (auto &LinkedOffenderNodeEdge : OffenderEdge->GetConnectedNode()->GetNodeEdges())
									{
										if (LinkedOffenderNodeEdge->GetConnectedNode() == Offender)
										{
											LinkedOffenderNodeEdge->ClearConnectedNode();
											break;
										}
									}
								}
								OffenderEdge->ClearConnectedNode();
							}
							Offender->Destroy();
						}
					}
					if (NextNode)
						NextNode->SpawnTraps();
				}

				if (!NextNode)
					continue;

				if (VisitedNodeList.Find(NextNode) != INDEX_NONE)
					continue;

				if (Depth <= 0)
					NodeEdge->ClearConnectedNode();

				NextNodeList.Push(NextNode);
			}

			if (Depth < 0)
				Node->Destroy();
		}

		--Depth;
	}

	VisitedNodeList.Reset();
}
*/

#include "Components/StaticMeshComponent.h"

bool AFPGNodeManager::CheckNodeBounds(class AFPGNode* Node, class AFPGNode* SpawningNode)
{
	auto Box = Node->GetNodeBox();

	for (TActorIterator<AFPGNode> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AFPGNode* OtherNode = *ActorItr;

		if (OtherNode == Node || OtherNode == SpawningNode)
			continue;
		
		auto OtherBox = OtherNode->GetNodeBox();

		if (OtherBox.Intersect(Box))//.Intersects(Sphere, 9.f))
			return false;
	}

	return true;
}


TArray<class AFPGNode*> AFPGNodeManager::GetOffendingBounds(class AFPGNode* Node, class AFPGNode* SpawningNode)
{
	TArray<class AFPGNode*> ret;
	auto Box = Node->GetNodeBox();
	for (TActorIterator<AFPGNode> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AFPGNode* OtherNode = *ActorItr;

		if (OtherNode == Node || OtherNode == SpawningNode)
			continue;

		auto OtherBox = OtherNode->GetNodeBox();

		if (OtherBox.Intersect(Box))//.Intersects(Sphere, 9.f))
			ret.Add(OtherNode);
	}

	return ret;
}


extern CORE_API uint32 GFrameNumber;

void AFPGNodeManager::OnEdgeTriggerBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// crappy overlap bug when going wayyy too fast causes stack overflows on SetRootNode
	static int32 LastFrameNumber = 0;
	if (LastFrameNumber == GFrameNumber)
		return;

	LastFrameNumber = GFrameNumber;

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController)
		return;

	APawn* PlayerPawn = PlayerController->GetPawn();
	
	if (PlayerPawn == nullptr)
		return;

	if (OtherActor != PlayerPawn)
		return;

	AFPGNode* Node = CastChecked<AFPGNode>(OverlappedComp->GetOwner());
	SetRootNode(Node);
}