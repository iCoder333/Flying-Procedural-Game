// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPGCannonProjectile.generated.h"

UCLASS()
class FPG_API AFPGCannonProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPGCannonProjectile();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float InitialSpeed;
	
	UPROPERTY(Category = Traps, EditDefaultsOnly)
	float ProjectileRadius;

	UPROPERTY(Category = Traps, EditDefaultsOnly)
	class UStaticMeshComponent* SphereMesh;

	UPROPERTY(EditDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement;
};
