// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskHandbrake.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include "AIWheeledVehicleController.h"
#include "ChaosWheeledVehicleMovementComponent.h"

EBTNodeResult::Type UBTTaskHandbrake::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());
	// If the AI controller is valid
	if (AIController)
	{
		// Get the blackboard component
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		bool Handbrake;

		// If the blackboard has a valid asset
		if (Blackboard->HasValidAsset())
		{
			// Get the handbrake value from the blackboard
			Handbrake = Blackboard->GetValueAsBool("HandbrakeValue");
		}
		else
		{
			Handbrake = false;
		}

		// Set the handbrake value
		AIController->VehicleMovementComp->SetHandbrakeInput(Handbrake);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}


void UBTTaskHandbrake::OnGameplayTaskActivated(UGameplayTask& task)
{

}