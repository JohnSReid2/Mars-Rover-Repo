// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMyUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // initialize the ProgressBar to 0%
    if (FuelBar)
    {
        FuelBar->SetPercent(0.0f);
    }
}

void UMyUserWidget::SetProgressBarPercentage(float InPercent)
{
    if (FuelBar)
    {
		// Set the progress bar percentage
        FuelBar->SetPercent(FMath::Clamp(InPercent, 0.0f, 1.0f));
    }
}

void UMyUserWidget::SetHealthBarPercentage(float InPercent)
{
	if (HealthBar)
	{
		// Set the health bar percentage
		HealthBar->SetPercent(FMath::Clamp(InPercent, 0.0f, 1.0f));
	}
}

void UMyUserWidget::SetFlareCount(int32 InCount)
{
	if (FlareCount)
	{
		// Set the flare count text
		FlareCount->SetText(FText::FromString(FString::FromInt(InCount)));
	}
}

void UMyUserWidget::SetPhotoCount(int32 InCount)
{
	if (PhotoCount)
	{
		// Set the photo count text
		PhotoCount->SetText(FText::FromString(FString::FromInt(InCount)));
	}
}

void UMyUserWidget::SetArtifactCount(int32 InCount)
{
	if (ArtifactCount)
	{
		// Set the artifact count text
		ArtifactCount->SetText(FText::FromString(FString::FromInt(InCount)));
	}
}

void UMyUserWidget::DisplayWinScreen()
{
	// Display the win screen
	if (TextEndCondition) {
		TextEndCondition->SetText(FText::FromString("You Win!!"));
		TextEndCondition->SetVisibility(ESlateVisibility::Visible);
	}
	if (TextRestart) {
		TextRestart->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMyUserWidget::DisplayLoseScreen()
{
	// Display the lose screen
	if (TextEndCondition) {
		TextEndCondition->SetText(FText::FromString("You Lose!!"));
		TextEndCondition->SetVisibility(ESlateVisibility::Visible);
	}
	if (TextRestart) {
		TextRestart->SetVisibility(ESlateVisibility::Visible);
	}
}
