// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VehicleStarterGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLESTARTER_API AVehicleStarterGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AVehicleStarterGameModeBase();

    // Function to record an artifact collection
    UFUNCTION(BlueprintCallable, Category = "Artifacts")
    void ArtifactCollected();

    // Getter for collected artifact count
    UFUNCTION(BlueprintCallable, Category = "Artifacts")
    int32 GetCollectedArtifactsCount() const;

	UFUNCTION(BlueprintCallable, Category = "Photos")
	void AddPhotoTaken(TArray<FString> PhotoName);


private:
    // Count of collected artifacts
    int32 CollectedArtifactsCount;

	// Array of strings to store the names of photos taken
	TArray<FString> PhotosTaken;

	int32 PhotosTakenCount;

    UFUNCTION()
	void CheckWinCondition();

	UFUNCTION()
	void RestartGame();
};


