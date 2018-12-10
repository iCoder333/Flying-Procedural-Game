// Copyright (c) 2017 - zerosum0x0

#include "FPGPendulumTrap.h"
#include "Components/InstancedStaticMeshComponent.h"

static FORCEINLINE const FVector SphericalToCartesianCoordinates(float Radius, float Polar, float Azimuthal)
{
	const float X = Radius * FMath::Sin(Polar) * FMath::Cos(Azimuthal);
	const float Y = Radius * FMath::Cos(Polar) * FMath::Sin(Azimuthal);
	const float Z = Radius * FMath::Cos(Polar) * -1;

	return FVector(X, Y, Z);
}

AFPGPendulumTrap::AFPGPendulumTrap()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	BobPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BobPivot"));
	BobPivot->SetupAttachment(RootComponent);

	WirePivot = CreateDefaultSubobject<USceneComponent>(TEXT("WirePivot"));
	WirePivot->SetupAttachment(RootComponent);

	BobMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BobMesh"));
	BobMesh->SetupAttachment(BobPivot);

	WireMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("WireMesh"));
	WireMesh->SetFlags(EObjectFlags::RF_Transactional);
	WireMesh->SetupAttachment(WirePivot);

	PendulumTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("PendulumTimeline"));
	PendulumTimeline->SetLooping(true);

	PendulumTimelineInterpFunction.BindDynamic(this, &AFPGPendulumTrap::PendulumTimelineUpdateCallback);

	PolarAngle = 60.f;
	WireLength = 300.f;
	PlayRate = 1.f;
}

void AFPGPendulumTrap::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	MakeWireInstances();
	UpdatePolarTransforms(PolarAngle);

	if (!PolarCurve)
		PolarCurve = NewObject<UCurveFloat>(this, TEXT("PlayCurve"));
}

void AFPGPendulumTrap::MakeWireInstances()
{
	WireMesh->ClearInstances();

	WirePivot->SetRelativeRotation(FRotator(PolarAngle, 0.f, 0.f));

	int32 WireCount = FMath::CeilToInt(WireLength / 327);

	for (int32 i = 0; i < WireCount; ++i)
	{
		const float TranslationZ = (i * 300 + 164) * -1;
		const float RotationYaw = (i * 90);

		const FTransform NewTransform(FRotator(0.f, RotationYaw, 0.f).Quaternion(), FVector(0.f, 0.f, TranslationZ));
		WireMesh->AddInstance(NewTransform);
	}

	const float ScaleZ = WireLength / (FMath::CeilToInt(WireLength / 329) * 329);
	WirePivot->SetRelativeScale3D(FVector(1.f, 1.f, ScaleZ));
}

void AFPGPendulumTrap::BeginPlay()
{
	Super::BeginPlay();

	PendulumTimeline->AddInterpFloat(PolarCurve, PendulumTimelineInterpFunction);
	PendulumTimeline->SetPlayRate(PlayRate);
	PendulumTimeline->Play();
}

void AFPGPendulumTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPGPendulumTrap::PendulumTimelineUpdateCallback(float PolarDelta)
{
	UpdatePolarTransforms(PolarAngle * PolarDelta);
}

void AFPGPendulumTrap::UpdatePolarTransforms(float NewPolar)
{
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system#Coordinate_system_conversions
	// Spherical to Cartesian Units
	const float Radial = WireLength;
	const float Polar = FMath::DegreesToRadians(NewPolar);
	//const float Azimuthal = FMath::DegreesToRadians(0.f);
	
	// optimize calculations for constant Azimuthal
	const float X = Radial * FMath::Sin(Polar); // * 1; //FMath::Cos(Azimuthal);
	const float Y = 0.f; // Radius * FMath::Cos(Polar) * FMath::Sin(Azimuthal);
	const float Z = Radial * FMath::Cos(Polar) * -1;
	
	BobPivot->SetRelativeLocation(FVector(X, Y, Z), true); // sweep collisions

	const FRotator NewRotation(NewPolar, 0.f, 0.f);
	BobPivot->SetRelativeRotation(NewRotation);
	WirePivot->SetRelativeRotation(NewRotation);
}

