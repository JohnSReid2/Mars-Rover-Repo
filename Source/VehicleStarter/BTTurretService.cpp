// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTurretService.h"
#include <EngineUtils.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "StarterWheeledVehiclePawn.h"
#include "RoverVehiclePawn.h"
#include "AIWheeledVehicleController.h"


void UBTTurretService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UWorld* world = OwnerComp.GetWorld();

	APawn* OwnerPawn;
	AAIWheeledVehicleController* AIVehicle = Cast<AAIWheeledVehicleController>(OwnerComp.GetAIOwner());
	OwnerPawn = AIVehicle->GetPawn();

	//get player location from the blackboard
	FVector PlayerLocation = AIVehicle->BlackboardComp->GetValueAsVector("PlayerLocation");

	if (PlayerLocation != FVector::ZeroVector)
	{
		FVector ForwardVector = OwnerPawn->GetActorForwardVector();
		FVector RightVector = OwnerPawn->GetActorRightVector();
		FVector TargetDirection = PlayerLocation - OwnerPawn->GetActorLocation();

		//Calculate angle between the forward vector of the vehicle and the direction to the player
		float TargetAngleValue = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, TargetDirection.GetSafeNormal())));
		if (FVector::CrossProduct(ForwardVector, TargetDirection.GetSafeNormal()).Z < 0)
		{
			TargetAngleValue = -TargetAngleValue;
		}

		
		//Get TurretRotation from the blackboard
		float TurretRotation = AIVehicle->BlackboardComp->GetValueAsFloat("TurretRotation");

		//Move the turret towards the player
		TurretRotation = FMath::FInterpTo(TurretRotation, TargetAngleValue, DeltaSeconds, 2.0f);

		//Set the new rotation in the blackboard
		AIVehicle->BlackboardComp->SetValueAsFloat("TurretRotation", TurretRotation);

		//Set the difference between the current rotation and the target rotation in the blackboard
		float RotationDifference = FMath::Abs(TurretRotation - TargetAngleValue);
		AIVehicle->BlackboardComp->SetValueAsFloat("RotationDifference", RotationDifference);

		//if the turret cooldown is greater than 0, decrease it
		float TurretCooldown = AIVehicle->BlackboardComp->GetValueAsFloat("TurretCooldown");
		if (TurretCooldown > 0)
		{
			TurretCooldown -= DeltaSeconds;
			AIVehicle->BlackboardComp->SetValueAsFloat("TurretCooldown", TurretCooldown);
		}

	}
}