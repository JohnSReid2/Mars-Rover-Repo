// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UProgressBar;
class UTextBlock;


/**
 * 
 */
UCLASS()
class VEHICLESTARTER_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


	// Function to set the progress bar percentage
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetProgressBarPercentage(float InPercent);

	// Function to set the health bar percentage
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetHealthBarPercentage(float InPercent);

	//Function to set the Flare count text
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetFlareCount(int32 InCount);

	//Function to set the Photo count text
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetPhotoCount(int32 InCount);

	//Function to set the Artifact count text
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetArtifactCount(int32 InCount);

	//Display the win screen
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DisplayWinScreen();

	//Display the lose screen
	UFUNCTION(BlueprintCallable, Category = "UI")
	void DisplayLoseScreen();
	
protected:

	//Fuel Bar
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* FuelBar;

	//Health Bar
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* HealthBar;

	//Flare Count
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* FlareCount;

	//Photo Count
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PhotoCount;

	//Artifact Count
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ArtifactCount;

	//Win Screen
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextEndCondition;

	//Restart Screen
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* TextRestart;
};
