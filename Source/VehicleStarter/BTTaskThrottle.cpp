// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskThrottle.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include "AIWheeledVehicleController.h"
#include "ChaosWheeledVehicleMovementComponent.h"

EBTNodeResult::Type UBTTaskThrottle::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		// get the blackboard component
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		float Throttle;
		
		if (Blackboard->HasValidAsset())
		{
			// get the value of the throttle from the blackboard
			Throttle = Blackboard->GetValueAsFloat("ThrottleValue");
		}
		else 
		{
			Throttle = FMath::RandRange(-1.0f, 1.0f);
		}
		
		// set the throttle and brake values
		if (Throttle > 0.0f)
		{
			AIController->VehicleMovementComp->SetThrottleInput(Throttle);
			AIController->VehicleMovementComp->SetBrakeInput(0.0f);
		}
		else 
		{
			AIController->VehicleMovementComp->SetThrottleInput(0);
			AIController->VehicleMovementComp->SetBrakeInput(-Throttle);
		}
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTaskThrottle::OnGameplayTaskActivated(UGameplayTask & task)
{

}


