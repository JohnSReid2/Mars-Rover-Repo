// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "RoverVehiclePawn.h"
#include "VehicleStarterGameModeBase.h"

// Sets default values
ACollectable::ACollectable()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Create the Collision Box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(MeshComponent);
	CollisionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Bind the Overlap Event
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACollectable::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * 25.0f;       //Scale our height by a factor of 20
	float DeltaRotation = DeltaTime * 25.0f;	//Rotate by 20 degrees per second
	NewRotation.Yaw += DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);

}

// Overlap Event Handler
void ACollectable::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//log to the console
	UE_LOG(LogTemp, Log, TEXT("Overlap Begin!"));

	// Check if the colliding actor is the player
	if (OtherActor && OtherActor != this)
	{
		ARoverVehiclePawn* player = Cast<ARoverVehiclePawn>(OtherActor);
		if (player) {
			OnCollected();
		}
	}
}

// Collected Function: Destroy the actor or perform any desired action
void ACollectable::OnCollected()
{
	// Get a reference to the Game Mode
	AVehicleStarterGameModeBase* GameMode = Cast<AVehicleStarterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		// Notify the Game Mode that an artifact has been collected
		GameMode->ArtifactCollected();
	}

	// Destroy this actor
	Destroy();
}