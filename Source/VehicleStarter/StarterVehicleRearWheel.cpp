// Fill out your copyright notice in the Description page of Project Settings.


#include "StarterVehicleRearWheel.h"

UStarterVehicleRearWheel::UStarterVehicleRearWheel()
{
	this->WheelRadius = 40.0f;
	this->WheelWidth = 40.0f;
	this->CorneringStiffness = 1000.0f;
	this->MaxSteerAngle = -60.0f;
	this->FrictionForceMultiplier = 4.0f;
	this->SideSlipModifier = 1.0f;
	this->SlipThreshold = 100.0f;
	this->SkidThreshold = 100.0f;

	this->bAffectedByHandbrake = true;
	this->bAffectedByBrake = false;
	this->bAffectedByEngine = true;
	this->bAffectedBySteering = true;
	this->AxleType = EAxleType::Rear;
}