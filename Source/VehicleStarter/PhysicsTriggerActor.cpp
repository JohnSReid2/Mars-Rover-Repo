// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsTriggerActor.h"
#include <Components/StaticMeshComponent.h>
#include "WheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

// Sets default values
APhysicsTriggerActor::APhysicsTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComponent->SetupAttachment(RootComponent);

	//Register OnBeginOverlap function as an OnActorBeginOverlap delegate
	OnActorBeginOverlap.AddDynamic(this, &APhysicsTriggerActor::OnBeginOverlap);
	//Register OnEndOverlap function as an OnActorEndOverlap delegate
	OnActorEndOverlap.AddDynamic(this, &APhysicsTriggerActor::OnEndOverlap);
	//Register OnHit function as an OnActorHitOverlap delegate
	OnActorHit.AddDynamic(this, &APhysicsTriggerActor::OnHit);

}

// Called when the game starts or when spawned
void APhysicsTriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APhysicsTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the actor overlaps with another actor
void APhysicsTriggerActor::OnBeginOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	AWheeledVehiclePawn* Vehicle = Cast<AWheeledVehiclePawn>(OtherActor);

	// If the overlapped actor is not a vehicle or the overlapped actor is the trigger actor itself, return
	if (!Vehicle || OtherActor == this) return;
	
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Begin Overlap"));
	
	UChaosVehicleMovementComponent* MoveComp = Vehicle->GetVehicleMovement();

	float ImpulseMagnitude = FMath::RandRange(500, 2500);
	FVector Impulse = Vehicle->GetActorForwardVector()*ImpulseMagnitude;
	
	Vehicle->GetMesh()->AddImpulse(Impulse, TEXT("Root"), true);
	Vehicle->UpdateAllReplicatedComponents();
}

void APhysicsTriggerActor::OnEndOverlap(AActor * OverlappedActor, AActor * OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("End Overlap"));
}

void APhysicsTriggerActor::OnHit(AActor * SelfActor, AActor * OtherActor, FVector NormalImpulse, const FHitResult & Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("A hit event"));
}

