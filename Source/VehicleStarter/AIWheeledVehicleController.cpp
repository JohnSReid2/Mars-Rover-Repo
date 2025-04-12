// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWheeledVehicleController.h"
#include "AIStarterWheeledVehiclePawn.h"
#include "AlienProjectile.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AAIWheeledVehicleController::AAIWheeledVehicleController()
{
	VehicleMovementComp = NULL;
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("AIVehicleBehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("AIVehicleBlackboardComp"));
}

void AAIWheeledVehicleController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	//Initialise controller members;
	AAIStarterWheeledVehiclePawn* AIVehicle = Cast<AAIStarterWheeledVehiclePawn>(InPawn);

	if (AIVehicle)
	{
		//Get the vehicle movement component and animation instance
		VehicleMovementComp = AIVehicle->GetVehicleMovementComponent();
		AnimInstance = Cast<URoverAnimationInstance>(AIVehicle->GetMesh()->GetAnimInstance());

		//Initialize the blackboard and start the behavior tree
		if (AIVehicle->BehaviorTree && AIVehicle->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AIVehicle->BehaviorTree->BlackboardAsset));
		}

		if (AIVehicle->BehaviorTree)
		{
			BehaviorTreeComp->StartTree(*AIVehicle->BehaviorTree);
		}
	}
}

void AAIWheeledVehicleController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AAIStarterWheeledVehiclePawn* AIVehicle = Cast<AAIStarterWheeledVehiclePawn>(GetPawn());

}