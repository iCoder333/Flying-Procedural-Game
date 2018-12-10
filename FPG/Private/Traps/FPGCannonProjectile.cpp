// Copyright (c) 2017 - zerosum0x0

#include "FPGCannonProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

AFPGCannonProjectile::AFPGCannonProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->OnComponentHit.AddDynamic(this, &AFPGCannonProjectile::OnSphereHit);
	//SphereMesh->SetSimulatePhysics(true);


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}


void AFPGCannonProjectile::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	float Scale = ProjectileRadius * 2 / 100;
	SetActorRelativeScale3D(FVector(Scale, Scale, Scale));

	ProjectileMovement->InitialSpeed = InitialSpeed;
	SphereMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AFPGCannonProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPGCannonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AFPGCannonProjectile::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SphereMesh->SetSimulatePhysics(true);
	SetLifeSpan(5.f);
}

