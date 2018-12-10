// Copyright (c) 2017 - zerosum0x0

#include "FPGNodeEdgeComponent.h"

#include "FPGNode.h"
#include "FPGPlayerPawn.h"
#include "FPGGameModeBase.h"

#include "Engine/World.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UFPGNodeEdgeComponent::UFPGNodeEdgeComponent()
{
	EdgeConnector = CreateDefaultSubobject<UArrowComponent>(TEXT("EdgeConnector"));
	//EdgeConnector->SetEditorScale(100.f);
	EdgeConnector->SetupAttachment(this);

	EdgeTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EdgeTrigger"));
	EdgeTrigger->BodyInstance.SetResponseToAllChannels(ECR_Ignore);
	EdgeTrigger->BodyInstance.SetResponseToChannel(ECC_Pawn, ECR_Overlap);
	EdgeTrigger->RelativeScale3D = FVector(10.f, 75.f, 50.f);
	EdgeTrigger->SetRelativeLocation(FVector(-1000.0, 0.f, 1000.f));
	EdgeTrigger->SetupAttachment(this);

	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UFPGNodeEdgeComponent::SnapToNodeEdge(class UFPGNodeEdgeComponent* That)
{
	AFPGNode* ThisNode = this->GetNode();
	AFPGNode* ThatNode = That->GetNode();

	// in C++ we can access the other guy's privates since we're the same class
	That->ConnectedNode = ThisNode;
	this->ConnectedNode = ThatNode;

	this->ConnectedNodeEdge = That;
	That->ConnectedNodeEdge = this;

	SnapParentToChildSubComponent(ThisNode, this->GetEdgeConnector(), That->GetEdgeConnector()->GetComponentTransform());
}

void UFPGNodeEdgeComponent::SnapParentToChildSubComponent(AActor* const Parent, const USceneComponent* const Offset, const FTransform& TowardTransform)
{
	// Step 0. Calculate and apply mirror rotation to update world space
	FRotator SnapRotator = TowardTransform.Rotator();
	const float ParentYaw = Parent->GetTransform().Rotator().Yaw;
	const float OffsetYaw = Offset->GetComponentTransform().Rotator().Yaw;
	const float TowardYaw = SnapRotator.Yaw;

	SnapRotator.Yaw -= 180.f;
	SnapRotator.Yaw += (ParentYaw - OffsetYaw);

	Parent->SetActorRotation(SnapRotator.Quaternion(), ETeleportType::TeleportPhysics);

	// Step 1. Calculate and apply distance offsets
	const FVector ParentTranslation = Parent->GetTransform().GetLocation();
	const FVector OffsetTranslation = Offset->GetComponentTransform().GetLocation();
	const FVector TowardTranslation = TowardTransform.GetLocation();

	const FVector SnapTranslation = TowardTranslation + (ParentTranslation - OffsetTranslation);

	Parent->SetActorLocation(SnapTranslation, false, nullptr, ETeleportType::TeleportPhysics);
}

