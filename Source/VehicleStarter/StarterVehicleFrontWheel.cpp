// Fill out your copyright notice in the Description page of Project Settings.


#include "StarterVehicleFrontWheel.h"

UStarterVehicleFrontWheel::UStarterVehicleFrontWheel()
{
	this->WheelRadius = 39.0f;
	this->WheelWidth = 35.0f;
	this->MaxSteerAngle = 60.0f;
	this->CorneringStiffness = 1000.0f;
	this->FrictionForceMultiplier = 3.0f;
	this->SideSlipModifier = 1.0f;
	this->SlipThreshold = 20.0f;
	this->SkidThreshold = 20.0f;

	this->bAffectedByHandbrake = true;
	this->bAffectedByBrake = false;
	this->bAffectedByEngine = true;
	this->bAffectedBySteering = true;
	this->AxleType = EAxleType::Front;
}