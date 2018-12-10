// Copyright (c) 2017 - zerosum0x0

#pragma once

#include "FPG.h"
#include "FPGSpawnBehavior.h"
#include "FPGTrap.h"

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"


#include "FPGSpawnGroup.generated.h"


UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class FPG_API UFPGSpawnGroup : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPGSpawnGroup();

	template<class T>
	T* Spawn()
	{
		if (SpawnTypes.Num() == 0)
			return nullptr;

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


		const FTransform BoxTransform = GetComponentTransform();
		const FVector Location = UKismetMathLibrary::RandomPointInBoundingBox(BoxTransform.GetLocation(), GetScaledBoxExtent());
		const FTransform SpawnTransform(BoxTransform.GetRotation(), Location);

		UClass* SpawnType = SpawnTypes[FMath::RandRange(0, SpawnTypes.Num() - 1)];

		AActor* Actor = GetWorld()->SpawnActor<AActor>(SpawnType, SpawnTransform, SpawnParameters);

		T* TypedActor = Cast<T>(Actor);

		if (TypedActor == nullptr)
			Actor->Destroy();

		return TypedActor;
	}

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	EFPGSpawnBehavior SpawnBehavior;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AActor>> SpawnTypes;
};
