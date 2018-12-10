// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FPGPlayerPawn.generated.h"

UCLASS()
class FPG_API AFPGPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AFPGPlayerPawn();

	virtual void Tick(float DeltaTime) override;

	void MoveAxisVertical(float Val);
	void MoveAxisHorizontal(float Val);
	void Kill();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:
	// tick events
	FORCEINLINE void AddAxisRotations(float Delta);
	FORCEINLINE void AddCurrentForwardSpeed(float Delta);
	FORCEINLINE void UpdateDeathCamera(float Delta);

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* SceneComponent;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(Category = Movement, VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USixDOFMovementComponent* Movement;

	bool bIsDying;

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
	float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
	float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Flight, EditAnywhere)
	float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Flight, EditAnywhere)
	float MinSpeed;

	/** Current forward speed */
	UPROPERTY(Category = Flight, EditAnywhere)
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	UPROPERTY(Category = Flight, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentPitchSpeed;

	/** Current pitch speed */
	UPROPERTY(Category = Score, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentDistance;

	UPROPERTY(Category = Score, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CurrentRings;


	float YawInput;
	float PitchInput;
	float MaxYawSpeed;
	float MaxPitchSpeed;
	float CurrentSpeed;
	float YawAcceleration;
	float PitchAcceleration;

	/** Current roll speed */
	float CurrentRollSpeed;
};
