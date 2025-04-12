// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStarterWheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "StarterVehicleFrontWheel.h"
#include "StarterVehicleRearWheel.h"
#include "Kismet/gameplaystatics.h"
#include "UObject/ConstructorHelpers.h"

AAIStarterWheeledVehiclePawn::AAIStarterWheeledVehiclePawn()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicles/AlienTank/Alien4"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	GetMesh()->SetSimulatePhysics(true);

	// Car animation blueprint
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicles/Rover/RiggedRover7_AnimBluePrint"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Set up the movement component
	UChaosWheeledVehicleMovementComponent* Vehicle4W = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	// Wheels setup
	FChaosWheelSetup Wheel_Setup;

	Wheel_Setup.AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);
	Wheel_Setup.WheelClass = UStarterVehicleFrontWheel::StaticClass();

	Wheel_Setup.BoneName = FName("Phys_Wheel_FL");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	Wheel_Setup.BoneName = FName("Phys_Wheel_FR");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	Wheel_Setup.WheelClass = UStarterVehicleRearWheel::StaticClass();
	Wheel_Setup.BoneName = FName("Phys_Wheel_BL");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	Wheel_Setup.BoneName = FName("Phys_Wheel_BR");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	// Generate a random name for the photo mode
	if (PhotoModeName == "")
	{
		PhotoModeName = FGuid::NewGuid().ToString();
	}
}

void AAIStarterWheeledVehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);
}

void AAIStarterWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
}

void AAIStarterWheeledVehiclePawn::Fire(float Rotation) {
	//Get the location and rotation of the pawn
	FVector PawnLocation = GetActorLocation();
	FRotator PawnRotation = GetActorRotation();
	
	// Calculate the fire rotation
	FRotator FireRotation = FRotator(0, Rotation, 0);
	FireRotation = PawnRotation + FireRotation;

	// Calculate the fire direction
	FVector FireDirection = FireRotation.Vector();

	// Offset slightly above and in front of the pawn
	FVector Offset = FVector(0.0f, 0.0f, 130.0f); // Adjust these values as needed
	FVector SpawnLocation = PawnLocation + FVector(0.0f, 0.0f, Offset.Z);


	// increase the horizontal component of the spawn location
	SpawnLocation += FireDirection * 100;


	// Spawn the projectile
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		//Make sure the projectile dosen't collide with the pawn
		SpawnParams.bNoFail = true;

		// Spawn the projectile
		AAlienProjectile* Projectile = World->SpawnActor<AAlienProjectile>(ProjectileClass, SpawnLocation, FireRotation, SpawnParams);
	}
}

FVector AAIStarterWheeledVehiclePawn::GetNearestPatrolPoint()
{
	// Get all the patrol points
	TArray<AActor*> PatrolPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PatrolActor, PatrolPoints);
	

	// Get the current location of the AI
	FVector CurrentLocation = GetActorLocation();

	FVector ReturnVector = FVector::ZeroVector;


	// Find the nearest patrol point
	float MinDistance = TNumericLimits<float>::Max();
	AActor* NearestPatrolPoint = nullptr;

	// If PatrolPoints is empty, return zero vector
	if (PatrolPoints.Num() == 0)
	{
		return ReturnVector;
	}

	// Find the nearest patrol point
	for (AActor* PatrolPoint : PatrolPoints)
	{
		float Distance = FVector::Dist(CurrentLocation, PatrolPoint->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestPatrolPoint = PatrolPoint;
		}
	}

	// Return the location of the nearest patrol point
	if (NearestPatrolPoint != nullptr)
	{
		ReturnVector = NearestPatrolPoint->GetActorLocation();
	}

	return ReturnVector;
}

FVector AAIStarterWheeledVehiclePawn::GetNearestFlarePoint()
{
	// Get all the flare points
	TArray<AActor*> FlarePoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FlareActor, FlarePoints);

	// Get the current location of the AI
	FVector CurrentLocation = GetActorLocation();

	FVector ReturnVector = FVector::ZeroVector;

	// Find the nearest flare point
	float MinDistance = TNumericLimits<float>::Max();
	AActor* NearestFlarePoint = nullptr;

	// If FlarePoints is empty, return zero vector
	if (FlarePoints.Num() == 0)
	{
		return ReturnVector;
	}

	// Find the nearest flare point
	for (AActor* FlarePoint : FlarePoints)
	{
		float Distance = FVector::Dist(CurrentLocation, FlarePoint->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NearestFlarePoint = FlarePoint;
		}
	}

	// Return the location of the nearest flare point
	if (NearestFlarePoint != nullptr)
	{
		ReturnVector = NearestFlarePoint->GetActorLocation();
	}

	return ReturnVector;
}