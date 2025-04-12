// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlareProjectile.h"
#include "WheeledVehiclePawn.h"
#include "RoverVehiclePawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTextRenderComponent;
class UInputComponent;
class USceneCaptureComponent2D;
class UMyUserWidget;

UCLASS()
class VEHICLESTARTER_API ARoverVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	/** SCene component for the In-Car view origin */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* InternalCameraBase;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* InternalCamera;

	// Scene capture component for the In-Car view
	UPROPERTY(Category = Camera, EditAnywhere)
	USceneCaptureComponent2D* InternalCameraCapture;

	// UI Widget to display the health and collected artifacts and photos, and camera view
	UPROPERTY(EditAnywhere, Category = "UI")
	UMyUserWidget* MyWidgetInstance;

public:
	ARoverVehiclePawn();

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* ThrusterFX;
	class UNiagaraComponent* NiagaraComp;

	/** Initial offset of incar camera */
	FVector InternalCameraOrigin;
	// Begin Pawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End Pawn interface

	// Begin Actor interface
	virtual void Tick(float Delta) override;
protected:
	virtual void BeginPlay() override;

public:
	// End Actor interface

	/** Handle pressing forwards */
	void MoveForward(float Val);

	/** Handle pressing right */
	void MoveRight(float Val);
	/** Handle Camera Look */
	void LookRight(float Val);
	/** Handle Camera Look */
	void LookUp(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
	/** Switch between cameras */
	void OnToggleCamera();
	/** Handle Thruster */
	void OnThrusterPressed();
	void OnThrusterReleased();

	// Handle Rotate Left
	void OnRotateLeftPressed();
	void OnRotateLeftReleased();

	// Handle Rotate Right
	void OnRotateRightPressed();
	void OnRotateRightReleased();

	// Handle Fire
	void FireProjectile();

	// Function to get the visible actors in the camera's view
	void GetVisibleActorsInRange(UCameraComponent* Camera, float MaxDistance, float CameraFoV, TArray<AActor*>& OutVisibleActors);

	// Function to damage the player
	void Damage(int DamageAmount);

	// Functions to set the Amount of collected artifacts and photos
	void SetCollectedArtifactCount(int Count);
	void SetPhotoCount(int Count);

	// Function to restart the game after the player dies
	void RestartGame();
	// Function to display the win screen after the player collects all the artifacts and photos
	void DisplayWinScreen();

	static const FName LookUpBinding;
	static const FName LookRightBinding;

	//Maximum duration of the thruster
	UPROPERTY(EditAnywhere)
	float MaxThrusterDuration;
	
	//Maximum cooldown of the thruster
	UPROPERTY(EditAnywhere)
	float MaxThrusterCooldown;
	
	// Turn rate for the camera
	UPROPERTY(EditAnywhere)
	float TurretRotationSpeed;

	// Max distance for the camera to detect objects
	UPROPERTY(EditAnywhere)
	float CameraTurretMaxDistance;

	// Max health of the player
	UPROPERTY(EditAnywhere)
	float MaxHealth;

	// Projectile class to spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class AFlareProjectile> ProjectileClass;

	
private:
	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;

	bool rotateLeftPressed;
	bool rotateRightPressed;

	float ThrusterCooldown;
	float ThrusterDuration;

	bool isThrusterPressed;
	float CurrentHealth;

	int FlareCount;
public:
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	/** Returns InternalCamera subobject **/
	FORCEINLINE UCameraComponent* GetInternalCamera() const { return InternalCamera; }
	/** Returns InternalCameraCapture subobject **/
	FORCEINLINE USceneCaptureComponent2D* GetInternalCameraCapture() const { return InternalCameraCapture; }
};
