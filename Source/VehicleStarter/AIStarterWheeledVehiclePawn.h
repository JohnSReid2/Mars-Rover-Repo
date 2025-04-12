// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AlienProjectile.h"
#include "AIStarterWheeledVehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLESTARTER_API AAIStarterWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

public:
	AAIStarterWheeledVehiclePawn();

	// Begin Actor interface
	virtual void Tick(float Delta) override;


	// Projectile class to spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AAlienProjectile> ProjectileClass;

	//Actor to patrol
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<class AActor > PatrolActor;

	//Flare to target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TSubclassOf<class AActor > FlareActor;

	//Name for photo mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FString PhotoModeName;

	// Fire a projectile.
	UFUNCTION()
	void Fire(float Rotation);

	// Get the nearest patrol point to the AI
	UFUNCTION()
	FVector GetNearestPatrolPoint();
	
	// Get the nearest flare point to the AI
	UFUNCTION()
	FVector GetNearestFlarePoint();
protected:
	virtual void BeginPlay() override;
};
