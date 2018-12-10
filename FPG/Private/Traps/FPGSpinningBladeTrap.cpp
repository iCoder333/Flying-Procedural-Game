// Copyright (c) 2017 - zerosum0x0

#include "FPGSpinningBladeTrap.h"
#include "Components/InstancedStaticMeshComponent.h"

AFPGSpinningBladeTrap::AFPGSpinningBladeTrap()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	BladeMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("BladeMesh"));
	BladeMesh->SetupAttachment(RootComponent); 
	BladeMesh->SetFlags(EObjectFlags::RF_Transactional);
		
	RevolutionRate = 3.f;
	DelayRate = 0.f;
	BladeCount = 2;
}

void AFPGSpinningBladeTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BladeMesh->ClearInstances(); // fix an editor bug, might be a better way?

	FQuat BQuat = BladeMesh->GetRelativeTransform().GetRotation();

	for (int32 i = 0; i < BladeCount; ++i)
	{
		const float Yaw = (360.0f / (float)BladeCount) * i;
		FQuat AQuat(FRotator(0.f, Yaw, 0.f));
		BladeMesh->AddInstance(FTransform(FRotator(BQuat * AQuat), FVector(0.f, 0.f, 0.f), BladeMesh->RelativeScale3D));
	}
}

void AFPGSpinningBladeTrap::BeginPlay()
{
	Super::BeginPlay();
}

void AFPGSpinningBladeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true) // IsOn
	{
		float BaseRotation = 360.f * RevolutionRate * DeltaTime;
		FRotator DeltaYaw(0.f, BaseRotation, 0.f);
		
		if (DelayRate != 0.f && PlayCurve)
		{
			static float Time = 0.f;
			Time += DeltaTime;
			DeltaYaw.Yaw = BaseRotation * PlayCurve->GetFloatValue(DeltaTime);
		}
		
		BladeMesh->AddRelativeRotation(DeltaYaw);
	}
}
