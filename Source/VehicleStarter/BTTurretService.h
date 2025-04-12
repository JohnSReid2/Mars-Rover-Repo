// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTTurretService.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLESTARTER_API UBTTurretService : public UBTService
{
	GENERATED_BODY()
	
private:
	class APawn* PlayerPawn;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
