// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTSteeringService.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLESTARTER_API UBTSteeringService : public UBTService
{
	GENERATED_BODY()

private:
	// Pointer to the player pawn
	class APawn* PlayerPawn;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
