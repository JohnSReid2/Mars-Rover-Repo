// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Collectable.generated.h"

UCLASS()
class VEHICLESTARTER_API ACollectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function to handle collection logic
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// Called when the actor is collected
	void OnCollected();

	// Static Mesh Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MeshComponent;

	// Collision Box
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collectible", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CollisionBox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
