// Fill out your copyright notice in the Description page of Project Settings.

#include <EngineUtils.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "StarterWheeledVehiclePawn.h"
#include "RoverVehiclePawn.h"
#include "AIWheeledVehicleController.h"
#include "AIStarterWheeledVehiclePawn.h"
#include "BTTargetService.h"

void UBTTargetService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UWorld* world = OwnerComp.GetWorld();

	TActorIterator<ARoverVehiclePawn> PlayerPawnIter(world);

	PlayerPawn = *PlayerPawnIter;

	if (PlayerPawn)
	{
		AAIWheeledVehicleController* AIVehicle = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());

		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		AIVehicle->BlackboardComp->SetValueAsBool("TargettingPlayer", true);

		//Get AI vehicle pawn of the controller
		AAIStarterWheeledVehiclePawn* AIVehiclePawn = Cast<AAIStarterWheeledVehiclePawn>(AIVehicle->GetPawn());

		//Get nearest patrol actor
		FVector PatrolLocation = AIVehiclePawn->GetNearestPatrolPoint();
		FVector FlareLocation = AIVehiclePawn->GetNearestFlarePoint();

		// If the AI vehicle is too far from the patrol point, set the player location to the patrol point
		if (PatrolLocation != FVector::ZeroVector)
		{
			float Distance = FVector::Dist(PatrolLocation, AIVehiclePawn->GetActorLocation());


			if (Distance > 10000.0f)
			{
				PlayerLocation = PatrolLocation;
				AIVehicle->BlackboardComp->SetValueAsBool("TargettingPlayer", false);
			}
		}

		// If the AI vehicle is too far from the flare point, set the player location to the flare point, if the flare point exists
		if (FlareLocation != FVector::ZeroVector)
		{
			float Distance = FVector::Dist(FlareLocation, AIVehiclePawn->GetActorLocation());

			if (Distance > 500.0f)
			{
				PlayerLocation = FlareLocation;
				AIVehicle->BlackboardComp->SetValueAsBool("TargettingPlayer", false);
			}
		}


		// Set the player location to the blackboard
		AIVehicle->BlackboardComp->SetValueAsVector("PlayerLocation", PlayerLocation);
	}
}
