// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleAnimationInstance.h"
#include "RoverAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLESTARTER_API URoverAnimationInstance : public UVehicleAnimationInstance
{
	GENERATED_BODY()

public:
	URoverAnimationInstance();

public:
    // ZRotation variable to control the rotation of a bone
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
    float ZRotation;

    // Function to set ZRotation from C++
    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void SetZRotation(float NewZRotation);
};