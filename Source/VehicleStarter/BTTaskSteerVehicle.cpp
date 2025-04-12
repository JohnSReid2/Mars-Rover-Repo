// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskSteerVehicle.h"
#include <Runtime/AIModule/Classes/BehaviorTree/BTTaskNode.h>
#include <Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeTypes.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "AIWheeledVehicleController.h"
#include "ChaosWheeledVehicleMovementComponent.h"


EBTNodeResult::Type UBTTaskSteerVehicle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIWheeledVehicleController* AIController = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

	if (AIController)
	{
		// get the blackboard component
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
		float Steering;
		
		if (Blackboard->HasValidAsset())
		{
			// get the value of the steering key
			Steering = Blackboard->GetValueAsFloat("SteeringValue");
		}
		else
		{
			Steering = FMath::RandRange(-1.0f, 1.0f);
		}
		
		// set the steering input
		AIController->VehicleMovementComp->SetSteeringInput(Steering);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

void UBTTaskSteerVehicle::OnGameplayTaskActivated(UGameplayTask & task)
{

}
