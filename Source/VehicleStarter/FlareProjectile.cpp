// Fill out your copyright notice in the Description page of Project Settings.


#include "FlareProjectile.h"

// Sets default values
AFlareProjectile::AFlareProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	// Set the root component to be the projectile's mesh.
	RootComponent = ProjectileMesh;

	if (!ProjectileMovementComponent) {
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 1.0f;
		InitialLifeSpan = 15.0f;

	}

}

// Called when the game starts or when spawned
void AFlareProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlareProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFlareProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

