// Fill out your copyright notice in the Description page of Project Settings.


#include "RoverAnimationInstance.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"

// Default constructor
URoverAnimationInstance::URoverAnimationInstance()
{
    // Initialize ZRotation to 0 by default
    ZRotation = 0.0f;
}


void URoverAnimationInstance::SetZRotation(float NewZRotation)
{
    ZRotation = NewZRotation;
}
