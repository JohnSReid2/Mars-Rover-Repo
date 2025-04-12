// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSteeringService.h"
#include <EngineUtils.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "StarterWheeledVehiclePawn.h"
#include "RoverVehiclePawn.h"
#include "AIWheeledVehicleController.h"


void UBTSteeringService::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	// Get the world
	UWorld* world = OwnerComp.GetWorld();

	
	APawn* OwnerPawn;
	// Get the AI controller
	AAIWheeledVehicleController* AIVehicle = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());
	// Get the player pawn
	OwnerPawn = AIVehicle->GetPawn();

	// Get the player location
	FVector PlayerLocation = AIVehicle->BlackboardComp->GetValueAsVector("PlayerLocation");

	// If the player location is not zero
	if (PlayerLocation != FVector::ZeroVector)
	{
		// Get the forward and right vectors of the AI vehicle
		FVector ForwardVector = OwnerPawn->GetActorForwardVector();
		FVector RightVector = OwnerPawn->GetActorRightVector();
		FVector TargetDirection = PlayerLocation - OwnerPawn->GetActorLocation();

		// Calculate the distance between the player and the AI vehicle
		float Distance = FMath::Sqrt(FVector::DistSquared(PlayerLocation, OwnerPawn->GetActorLocation()));
		float Throttle = FMath::Clamp<float>(0.0f, 1.0f, Distance / 5000.0f);
		bool Handbrake = false;
		
		// Normalize the target direction
		TargetDirection.Normalize(1.05f);

		// Calculate the angle between the forward vector and the target direction
		float AngleValue = FVector::DotProduct(ForwardVector, TargetDirection);
		float RightValue = FVector::DotProduct(RightVector, TargetDirection);

		float SteerValue = 0.0f; //-RightValue;// < 0.0f ? 1.0f - AngleValue : -1.0f + AngleValue;
		
		// Steer the AI vehicle
		if (RightValue < 0.0f)

		{
			SteerValue = AngleValue < 0.0f ? -1.0f : RightValue;
		}
		else
		{
			SteerValue = AngleValue < 0.0f ? 1.0f : RightValue;
		}

		// If the distance is less than 1500, reverse
		if (Distance <= 1500.0f)
		{
			// If too close, reverse
			Throttle = -1.0f;
			SteerValue = -SteerValue;
			Handbrake = false;
		}
		// If the distance is between 1500 and 3000, stop
		else if (Distance > 1500 and Distance <= 3000) {
			Throttle = 0.0f;
			SteerValue = 0.0f;
			Handbrake = true;
		}
		// If the distance is greater than 3000, move forward
		else
		{
			Throttle = 1.0f;
			Handbrake = false;
		}

		// Set the steering value, throttle value, and handbrake value in the blackboard
		AIVehicle->BlackboardComp->SetValueAsFloat("SteeringValue", SteerValue);
		AIVehicle->BlackboardComp->SetValueAsFloat("ThrottleValue", Throttle);
		AIVehicle->BlackboardComp->SetValueAsBool("HandbrakeValue", Handbrake);
	}
}
