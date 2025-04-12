// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskFireTurret.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include "AIWheeledVehicleController.h"
#include "AIStarterWheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"


EBTNodeResult::Type UBTTaskFireTurret::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		// Get the blackboard component from the owner component
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		float Rotation;
		float Cooldown;
		float RotationDifference;
		bool TargettingPlayer;
		
		// If the blackboard has a valid asset
		if (Blackboard->HasValidAsset())
		{
			// Get the values from the blackboard
			Rotation = Blackboard->GetValueAsFloat("TurretRotation");
			Cooldown = Blackboard->GetValueAsFloat("TurretCooldown");
			RotationDifference = Blackboard->GetValueAsFloat("RotationDifference");
			TargettingPlayer = Blackboard->GetValueAsBool("TargettingPlayer");

		}
		else
		{
			Rotation = FMath::RandRange(-1.0f, 1.0f);
			Cooldown = 0.0f;
			RotationDifference = 0.0f;
		}
		//Get the player location from the blackboard
		FVector PlayerLocation = Blackboard->GetValueAsVector("PlayerLocation");

		// Get the owner pawn
		APawn* OwnerPawn = AIController->GetPawn();
		float Distance = FMath::Sqrt(FVector::DistSquared(PlayerLocation, OwnerPawn->GetActorLocation()));

		// If the player is within 8000 units and the turret is targetting the player
		if (Distance < 8000 and TargettingPlayer)
		{
			// If the turret is facing the player
			if (RotationDifference < 5)
			{
				// If the turret is not on cooldown
				if (Cooldown <= 0)
				{
					
					AIController->GetPawn();
					
					//If the AI is a starter wheeled vehicle
					if (AIController->GetPawn()->IsA(AAIStarterWheeledVehiclePawn::StaticClass()))
					{
						// Cast the AI to a starter wheeled vehicle
						AAIStarterWheeledVehiclePawn* StarterWheeledVehicle = Cast<AAIStarterWheeledVehiclePawn>(AIController->GetPawn());
						// Fire the turret
						StarterWheeledVehicle->Fire(Rotation);
					}
					
					
					// Set the cooldown to 4 seconds
					Cooldown = 4.0f;
					// Set the new cooldown in the blackboard
					Blackboard->SetValueAsFloat("TurretCooldown", Cooldown);
				}
			}
			
		}


		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


void UBTTaskFireTurret::OnGameplayTaskActivated(UGameplayTask& task)
{
	// Do nothing
}