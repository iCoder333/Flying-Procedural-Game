// Copyright (c) 2017 - zerosum0x0

#include "FPGPlayerPawn.h"
#include "FPGPlayerController.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Vendor/SixDOFMovementComponent.h"
//#include "SixDOFMovementComponent.h"

AFPGPlayerPawn::AFPGPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetCapsuleSize(30.f, 30.f, false);
	RootComponent = CapsuleComponent;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->BodyInstance.bUseCCD = false;

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CapsuleComponent);	// Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->bEnableCameraRotationLag = false;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);	// Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	Movement = CreateDefaultSubobject<USixDOFMovementComponent>(TEXT("Movement"));

	// auto navigate toward camera (allows us to simulate camera lag)
	Movement->LookAt = false;
	/*
	Movement->LookAtMode = EAutopilotMode::LM_Direction;
	Movement->SetLookAtVector(FVector(1.0, 0.f, 0.f));
	Movement->LookAtSensitivityPitch = 1.f;
	Movement->LookAtSensitivityYaw = 1.f;
	Movement->LookAtSensitivityRoll = 1.f;
	*/
	// smooth movement out
	Movement->RotationDamping = true;
	Movement->RotationDampingPitch = 0.5f;
	Movement->RotationDampingYaw = 0.5f;
	Movement->RotationDampingRoll = 0.5f;

	// level out the roll
	Movement->Autolevel = false;
	Movement->SetAutolevelUpVector(FVector(0.f, 0.f, 1.f));
	Movement->AutolevelSensitivityPitch = 0.f;
	Movement->AutolevelSensitivityYaw = 0.f;
	Movement->AutolevelSensitivityRoll = 0.6f;

	// disable effects of gravity
	Movement->UseGravity = false;
	Movement->UseGravityVector = false;

	// Always move forward
	Movement->MaxSpeedForward = 1000.f;
	Movement->SetForwardInput(1.f);
											 // Set handling parameters
	
	Acceleration = 15.f; // +900cm/s/60s
	MaxSpeed = 5000.f;
	///TurnSpeed = 200.f;
	///MinSpeed = 1000.f;
	///CurrentForwardSpeed = 1000.f;

	// move to playerstate?
	///CurrentDistance = 0.f;
	///CurrentRings = 0.f;
	bIsDying = false;
}

void AFPGPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AFPGPlayerPawn::Tick(float DeltaSeconds)
{
	///const float AbsolutePitch = FMath::Abs(Camera->GetComponentTransform().Rotator().Pitch);
	///const bool Horizontal = AbsolutePitch < 50.f;
	const bool NoInputs = FMath::IsNearlyZero(Movement->PitchInput) && FMath::IsNearlyZero(Movement->YawInput);

	Movement->Autolevel = NoInputs;// && Horizontal;

	float NewSpeed = FMath::Min(Movement->MaxSpeedForward + Acceleration * DeltaSeconds, MaxSpeed);
	Movement->MaxSpeedForward = NewSpeed;

	if (Movement->Autolevel)
		Movement->AutolevelSensitivityRoll = 5.f; //Movement->MaxSpeedForward / 1000.f;


	// Call any parent class Tick implementation
	Super::Tick(DeltaSeconds);
}

void AFPGPlayerPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Deflect along the surface when we collide.
	//FRotator CurrentRotation = GetActorRotation();
	//SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
		
	this->Kill();
}

void AFPGPlayerPawn::Kill()
{
	//this->bTearOff = true;
	//this->DetachFromControllerPendingDestroy();
	Movement->Deactivate();

	// make it all ragdolly
	SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
	SkeletalMesh->SetAllBodiesSimulatePhysics(true);
	//SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->WakeAllRigidBodies();
	SkeletalMesh->bBlendPhysics = true;

	// Apply an impulse in the direction we are heading  
	/*
	FVector ImpulseRotation = Camera->GetComponentRotation().Vector();
	static const float ImpulseAmount = CurrentForwardSpeed * 10.f;
	SkeletalMesh->AddImpulse(ImpulseRotation * ImpulseAmount);
	*/
	bIsDying = true;

	AFPGPlayerController* PlayerController = Cast<AFPGPlayerController>(GetController());

	if (PlayerController != nullptr)
	{
		PlayerController->OnDeath();
	}
}

void AFPGPlayerPawn::MoveAxisHorizontal(float Val)
{
	Movement->SetYawInput(Val);

	if (FMath::Abs(Val) > 0.2f)
		Movement->SetRollInput(-Val * 0.3);

	///SpringArm->AddLocalRotation(FRotator(0.f, Val / Movement->MaxSpeedYaw * 5, 0.f));
}

void AFPGPlayerPawn::MoveAxisVertical(float Val)
{
	Movement->SetPitchInput(Val);

	//SpringArm->AddLocalRotation(FRotator(Val / Movement->MaxSpeedPitch * 3, 0.f, 0.f));
	/*
	float TargetPitchSpeed = (Val * TurnSpeed * -1.f);
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
	*/
}



void AFPGPlayerPawn::AddCurrentForwardSpeed(float DeltaSeconds)
{
	CurrentForwardSpeed += Acceleration * DeltaSeconds;
	CurrentForwardSpeed = FMath::Min(CurrentForwardSpeed, MaxSpeed);

	float Distance = CurrentForwardSpeed * DeltaSeconds;
	const FVector LocalMove = FVector(Distance, 0.f, 0.f);
	AddActorLocalOffset(LocalMove, true);

	CurrentDistance += Distance;
}

void AFPGPlayerPawn::AddAxisRotations(float DeltaSeconds)
{
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	// Rotate plane
	AddActorLocalRotation(DeltaRotation);
}

void AFPGPlayerPawn::UpdateDeathCamera(float DeltaSeconds)
{
	auto VectorDelta = FMath::VInterpTo(CapsuleComponent->GetComponentLocation(), SkeletalMesh->GetComponentLocation(), DeltaSeconds, 3.0f);
	auto RotationDelta = FMath::RInterpTo(CapsuleComponent->GetComponentRotation(), FRotator(-30.0f, 0.0f, 0.f), DeltaSeconds, 3.f);
	FTransform f(RotationDelta, VectorDelta, CapsuleComponent->RelativeScale3D);
	CapsuleComponent->SetWorldTransform(f, false);
}