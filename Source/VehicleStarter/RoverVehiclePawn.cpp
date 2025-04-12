// Fill out your copyright notice in the Description page of Project Settings.
#include "RoverVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "StarterVehicleFrontWheel.h"
#include "StarterVehicleRearWheel.h"
#include "StarterVehicleMidWheel.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.h"
#include "RoverAnimationInstance.h"
#include "FlareProjectile.h"
#include "VehicleStarterGameModeBase.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

ARoverVehiclePawn::ARoverVehiclePawn()
{
	// Car mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/Vehicles/Rover/RiggedRover7"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);
	GetMesh()->SetSimulatePhysics(true);

	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/Vehicles/Rover/RiggedRover7_AnimBluePrint"));
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	// Set up wheels animation bones and class
	UChaosWheeledVehicleMovementComponent* Vehicle4W = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	Vehicle4W->bEnableCenterOfMassOverride = true;
	Vehicle4W->CenterOfMassOverride = FVector(0.0f, 0.0f, -100.0f);

	FChaosWheelSetup Wheel_Setup;
	Wheel_Setup.AdditionalOffset = FVector(0.0f, 0.0f, 14.0f);

	Wheel_Setup.WheelClass = UStarterVehicleFrontWheel::StaticClass();
	Wheel_Setup.BoneName = FName("Wheel-FL");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);
	Wheel_Setup.BoneName = FName("Wheel-FR");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	Wheel_Setup.WheelClass = UStarterVehicleMidWheel::StaticClass();
	Wheel_Setup.BoneName = FName("Wheel-ML");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);
	Wheel_Setup.BoneName = FName("Wheel-MR");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	Wheel_Setup.WheelClass = UStarterVehicleRearWheel::StaticClass();
	Wheel_Setup.BoneName = FName("Wheel-RL");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);
	Wheel_Setup.BoneName = FName("Wheel-RR");
	Vehicle4W->WheelSetups.Add(Wheel_Setup);

	// Create a spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 200.f);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;
	SpringArm->bUsePawnControlRotation= true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = true;

	// Create camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	// Create Camera Turret Components
	InternalCameraOrigin = FVector(0.0f, -40.0f, 120.0f);

	InternalCameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("InternalCameraBase"));
	InternalCameraBase->SetRelativeLocation(InternalCameraOrigin);
	InternalCameraBase->SetupAttachment(GetMesh());

	InternalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InternalCamera"));
	InternalCamera->bUsePawnControlRotation = false;
	InternalCamera->FieldOfView = 90.f;
	InternalCamera->SetupAttachment(InternalCameraBase);

	// Create camera capture component
	InternalCameraCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("InternalCameraCapture"));
	InternalCameraCapture->ProjectionType = ECameraProjectionMode::Perspective;
	InternalCameraCapture->FOVAngle = 90.0f;
	InternalCameraCapture->SetupAttachment(InternalCamera);

	// Initialize thruster variables
	MaxThrusterDuration = 1.5f;
	ThrusterDuration = 0.0f;

	MaxThrusterCooldown = 3.0f;
	ThrusterCooldown = 0.0f;

	// Initialize turret variables
	CameraTurretMaxDistance = 4000.0f;

	// Initialize the health variables
	MaxHealth = 5;
	CurrentHealth = MaxHealth;
	
	// Initialize the flare variables
	FlareCount = 3;
}

void ARoverVehiclePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ARoverVehiclePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARoverVehiclePawn::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ARoverVehiclePawn::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ARoverVehiclePawn::LookRight);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ARoverVehiclePawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ARoverVehiclePawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("SwitchCamera", IE_Pressed, this, &ARoverVehiclePawn::OnToggleCamera);

	PlayerInputComponent->BindAction("Thruster", IE_Pressed, this, &ARoverVehiclePawn::OnThrusterPressed);
	PlayerInputComponent->BindAction("Thruster", IE_Released, this, &ARoverVehiclePawn::OnThrusterReleased);

	PlayerInputComponent->BindAction("TurretLeft", IE_Pressed, this, &ARoverVehiclePawn::OnRotateLeftPressed);
	PlayerInputComponent->BindAction("TurretLeft", IE_Released, this, &ARoverVehiclePawn::OnRotateLeftReleased);
	PlayerInputComponent->BindAction("TurretRight", IE_Pressed, this, &ARoverVehiclePawn::OnRotateRightPressed);
	PlayerInputComponent->BindAction("TurretRight", IE_Released, this, &ARoverVehiclePawn::OnRotateRightReleased);

	PlayerInputComponent->BindAction("Flare", IE_Pressed, this, &ARoverVehiclePawn::FireProjectile);
}

void ARoverVehiclePawn::MoveForward(float Val)
{
	//Set the throttle and brake based on the input value
	if (Val > 0.0f)
	{
		GetVehicleMovementComponent()->SetThrottleInput(Val);
		GetVehicleMovementComponent()->SetBrakeInput(0);
	}
	else
	{
		GetVehicleMovementComponent()->SetBrakeInput(-Val);
		GetVehicleMovementComponent()->SetThrottleInput(0);
	}
}

void ARoverVehiclePawn::MoveRight(float Val)
{
	// Set the steering input based on the input value
	GetVehicleMovementComponent()->SetSteeringInput(Val);

	// Align the camera to face the same direction as the vehicle
	FRotator VehicleRotation = GetActorRotation();
	SpringArm->SetRelativeRotation(FRotator(SpringArm->GetRelativeRotation().Pitch, VehicleRotation.Yaw, 0.0f));
}

void ARoverVehiclePawn::LookUp(float Val)
{
	// Clamp the input to prevent excessive rotation
	Val = FMath::Clamp(Val, -5.0f, 5.0f);

	// Define rotation limits
	FRotator NewRotation = SpringArm->GetRelativeRotation();
	float NewPitch = FMath::Clamp(NewRotation.Pitch + Val, -40.0f, 80.0f); // Adjust -80 and 80 as needed

	// Apply the clamped rotation
	SpringArm->SetRelativeRotation(FRotator(NewPitch, NewRotation.Yaw, NewRotation.Roll));
}

void ARoverVehiclePawn::LookRight(float Val)
{
	// Clamp the input to prevent excessive rotation
	Val = FMath::Clamp(Val, -5.0f, 5.0f);
}

void ARoverVehiclePawn::OnHandbrakePressed()
{
	// Pull the handbrake
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ARoverVehiclePawn::OnHandbrakeReleased()
{
	// Release the handbrake
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
	GetMesh()->AddImpulse(FVector(0, 0, 1000), TEXT("Vehicle_Base"), true); 
	UpdateAllReplicatedComponents();
}

void ARoverVehiclePawn::OnToggleCamera()
{
	// Toggle the camera between the internal and external cameras
	TArray<AActor*> OutVisibleActors;
	GetVisibleActorsInRange(InternalCamera, CameraTurretMaxDistance, InternalCamera->FieldOfView, OutVisibleActors);
}

void ARoverVehiclePawn::OnThrusterPressed(){isThrusterPressed = true;}
void ARoverVehiclePawn::OnThrusterReleased(){isThrusterPressed = false;}
void ARoverVehiclePawn::OnRotateLeftPressed(){rotateLeftPressed = true;}
void ARoverVehiclePawn::OnRotateLeftReleased(){rotateLeftPressed = false;}
void ARoverVehiclePawn::OnRotateRightPressed(){rotateRightPressed = true;}
void ARoverVehiclePawn::OnRotateRightReleased(){rotateRightPressed = false;}



void ARoverVehiclePawn::Tick(float Delta)
{
	Super::Tick(Delta);
	// If the thruster is pressed and the thruster duration is greater than 0, add force to the vehicle
	if (isThrusterPressed and ThrusterDuration > 0.0f) {


		GetMesh()->AddForce(FVector(0, 0, 800), TEXT("Root"), true);
		
		TArray<FName> BoneNames;
		// Add the bone names to the array
		BoneNames.Add(FName("Wheel-FR")); BoneNames.Add(FName("Wheel-FL")); 
		BoneNames.Add(FName("Wheel-MR")); BoneNames.Add(FName("Wheel-ML"));  
		BoneNames.Add(FName("Wheel-RR")); BoneNames.Add(FName("Wheel-RL"));

		// For each bone name in the array, display particle effects
		for (FName BoneName : BoneNames) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				ThrusterFX,               // The Niagara system asset.
				this->RootComponent,       // Attach to this skeletal mesh.
				BoneName,                    // Name of the bone to attach to.
				FVector::ZeroVector,         // Location offset relative to the bone.
				FRotator::ZeroRotator,       // Rotation offset relative to the bone.
				EAttachLocation::KeepRelativeOffset, // Attachment rule.
				true                         // Auto destroy.
			);
		}
		// Decrease the thruster duration and reset the cooldown
		ThrusterDuration -= Delta; 
		ThrusterCooldown = 0.0f;
	}
	// If the thruster is not pressed and the thruster duration is greater than 0, increase the cooldown
	else {
		ThrusterCooldown += Delta;
		// If the thruster duration is less than the max duration and the cooldown is greater than the 
		// max cooldown, increase the duration
		if (ThrusterDuration < MaxThrusterDuration && ThrusterCooldown > MaxThrusterCooldown) {
			ThrusterDuration += Delta;
		}
	}

	// Set the thruster percentage on the widget
	MyWidgetInstance->SetProgressBarPercentage(ThrusterDuration / MaxThrusterDuration);

	// Rotate the turret
	URoverAnimationInstance* AnimInstance = Cast<URoverAnimationInstance>(GetMesh()->GetAnimInstance());
	if (rotateLeftPressed) {
		// Rotate the internal camera base
		InternalCameraBase->AddLocalRotation(FRotator(0, -TurretRotationSpeed * Delta , 0));
		if (AnimInstance)
		{
			// Rotate the animation instance
			AnimInstance->SetZRotation(AnimInstance->ZRotation + (-TurretRotationSpeed * Delta));
		}
	}
	if (rotateRightPressed) {
		// Rotate the internal camera base
		InternalCameraBase->AddLocalRotation(FRotator(0, TurretRotationSpeed * Delta , 0));
		if (AnimInstance)
		{
			// Rotate the animation instance
			AnimInstance->SetZRotation(AnimInstance->ZRotation + (TurretRotationSpeed * Delta));
		}
	}
}

void ARoverVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	// Create and add the widget to the viewport
	if (UClass* WidgetClass = LoadClass<UMyUserWidget>(nullptr, TEXT("/Game/VehicleTemplate/Blueprints/UIWidget.UIWidget_C")))
	{
		MyWidgetInstance = CreateWidget<UMyUserWidget>(GetWorld(), WidgetClass);
		if (MyWidgetInstance)
		{
			MyWidgetInstance->AddToViewport();
		}
	}
	// Set the health bar percentage on the widget
	MyWidgetInstance->SetHealthBarPercentage(CurrentHealth / MaxHealth);
}


void ARoverVehiclePawn::GetVisibleActorsInRange(UCameraComponent* InputCamera, float MaxDistance, float CameraFoV, TArray<AActor*>& OutVisibleActors)
{
	// Ensure a valid Camera Component
	if (!InputCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera component is invalid."));
		return;
	}

	// Get camera position and forward vector
	FVector CameraLocation = InputCamera->GetComponentLocation();
	FVector CameraForward = InputCamera->GetForwardVector();
	float HalfFoVRadians = FMath::DegreesToRadians(CameraFoV / 2.0f); // Convert FoV degrees to radians

	// Get all actors in the world
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	// Clear output array
	OutVisibleActors.Empty();

	// Iterate through all actors
	for (AActor* Actor : AllActors)
	{
		if (!Actor || Actor == this) // Skip invalid or self
			continue;

		FVector ActorLocation = Actor->GetActorLocation();

		// Calculate distance to the camera
		float Distance = FVector::Dist(CameraLocation, ActorLocation);
		if (Distance > MaxDistance)
			continue; // Skip actors out of range

		// Check if the actor is in front of the camera (Dot Product)
		FVector DirectionToActor = (ActorLocation - CameraLocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(CameraForward, DirectionToActor);
		if (DotProduct < 0.0f) // Actor is behind the camera
			continue;

		// Check if the actor is within the camera's FoV
		float AngleToActor = FMath::Acos(DotProduct); // Angle between camera forward and direction to actor
		if (AngleToActor > HalfFoVRadians) // Outside of the FoV
			continue;

		// Perform a Line Trace (Visibility Check)
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);  // Ignore self
		QueryParams.AddIgnoredActor(InputCamera->GetOwner()); // Ignore camera's owner

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			CameraLocation,
			ActorLocation,
			ECC_Visibility,
			QueryParams
		);
		if (!bHit || HitResult.GetActor() == Actor)
		{
			// Actor is visible to the camera
			OutVisibleActors.Add(Actor);
		}
	}
	
	// Add the photo mode names to the game mode
	TArray<FString> PhotoModeNames;
	for (AActor* Actor : OutVisibleActors)
	{
		// Get the PhotoModeName property from the actor
		FProperty* Property = Actor->GetClass()->FindPropertyByName("PhotoModeName");
		// If the property is valid, add the value to the array
		if (Property && Property->IsA<FStrProperty>())
		{
			// Cast the property to a string property
			FStrProperty* StringProperty = CastField<FStrProperty>(Property);
			// Get the property value from the actor
			FString PropertyValue = StringProperty->GetPropertyValue_InContainer(Actor);
			// Add the property value to the array
			PhotoModeNames.Add(PropertyValue);
		}
	}

	// Add the photo mode names to the game mode
	AVehicleStarterGameModeBase* GameMode = Cast<AVehicleStarterGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->AddPhotoTaken(PhotoModeNames);
	}

}


void ARoverVehiclePawn::FireProjectile()
{
	// Get the location of the interal camera
	FVector CameraLocation = InternalCamera->GetComponentLocation();

	// Get the rotation of the internal camera
	FRotator CameraRotation = InternalCamera->GetComponentRotation();

	// Fire the projectile in the direction the camera is facing
	FVector FireDirection = CameraRotation.Vector();

	// Get the world
	UWorld* const World = GetWorld();
	// If the world is valid and the flare count is greater than 0, spawn a flare projectile
	if (World != NULL and FlareCount > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		//Make sure the projectile dosen't collide with the pawn
		SpawnParams.bNoFail = true;

		AFlareProjectile* Projectile = World->SpawnActor<AFlareProjectile>(ProjectileClass, CameraLocation, CameraRotation, SpawnParams);
		FlareCount--;

		MyWidgetInstance->SetFlareCount(FlareCount);
	}

}

void ARoverVehiclePawn::Damage(int DamageAmount)
{
	// Decrease the health of the vehicle
	CurrentHealth -= DamageAmount;
	// Set the health bar percentage on the widget
	MyWidgetInstance->SetHealthBarPercentage(CurrentHealth / MaxHealth);
	// If the health is less than or equal to 0, display the lose screen
	if (CurrentHealth <= 0)
	{
		//Display Game Over
		UE_LOG(LogTemp, Warning, TEXT("Game Over"));

		MyWidgetInstance->DisplayLoseScreen();

		//Restart the level
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &ARoverVehiclePawn::RestartGame, 5.0f, false);

	}
		
}

void ARoverVehiclePawn::DisplayWinScreen()
{
	MyWidgetInstance->DisplayWinScreen();
}

void ARoverVehiclePawn::SetCollectedArtifactCount(int Count)
{
	MyWidgetInstance->SetArtifactCount(Count);
}

void ARoverVehiclePawn::SetPhotoCount(int Count)
{
	MyWidgetInstance->SetPhotoCount(Count);
}

void ARoverVehiclePawn::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

#undef LOCTEXT_NAMESPACE