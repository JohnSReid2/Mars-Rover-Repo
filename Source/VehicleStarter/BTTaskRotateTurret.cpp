// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskRotateTurret.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include "AIWheeledVehicleController.h"
#include "ChaosWheeledVehicleMovementComponent.h"


EBTNodeResult::Type UBTTaskRotateTurret::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get the AI controller
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

	// If the AI controller is valid
	if (AIController)
	{
		
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		float Rotation;
		bool TargettingPlayer;

		if (Blackboard->HasValidAsset())
		{
			// Get the values from the blackboard
			Rotation = Blackboard->GetValueAsFloat("TurretRotation");
			TargettingPlayer = Blackboard->GetValueAsBool("TargettingPlayer");
		}
		else
		{
			Rotation = FMath::RandRange(-1.0f, 1.0f);
		}

		//Get the player location from the blackboard
		FVector PlayerLocation = Blackboard->GetValueAsVector("PlayerLocation");

		// Get the distance between the player and the AI
		APawn* OwnerPawn = AIController->GetPawn();
		float Distance = FMath::Sqrt(FVector::DistSquared(PlayerLocation, OwnerPawn->GetActorLocation()));

		// If the player is within 8000 units and the AI is targetting the player, set the rotation of the turret
		if (Distance < 8000 and TargettingPlayer)
		{
			AIController->AnimInstance->SetZRotation(Rotation);
		}
		

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTaskRotateTurret::OnGameplayTaskActivated(UGameplayTask& task)
{
	// Do nothing
}