// Fill out your copyright notice in the Description page of Project Settings.

#include "VehicleStarter.h"
#include "RoverVehiclePawn.h"
#include "AlienProjectile.h"

// Sets default values
AAlienProjectile::AAlienProjectile()
{
	

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	// Set the root component to be the projectile's mesh.
	RootComponent = ProjectileMesh;


	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	

	// Event called when component hits something.
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AAlienProjectile::OnHit);
	

}

// Called when the game starts or when spawned
void AAlienProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAlienProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AAlienProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void AAlienProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Log on hit

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	
		//If other actor is the RoverVehiclePawn class, call the TakeDamage function
		if (OtherActor->IsA(ARoverVehiclePawn::StaticClass()))
		{
			ARoverVehiclePawn* Rover = Cast<ARoverVehiclePawn>(OtherActor);
			Rover->Damage(1);
		}
	}
	Destroy();
}