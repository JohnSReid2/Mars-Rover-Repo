// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleStarterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "RoverVehiclePawn.h"
#include "MyUserWidget.h"
#include "StarterWheeledVehiclePawn.h"


AVehicleStarterGameModeBase::AVehicleStarterGameModeBase()
{
	DefaultPawnClass = AStarterWheeledVehiclePawn::StaticClass();
	CollectedArtifactsCount = 0;
	PhotosTakenCount = 0;
}

void AVehicleStarterGameModeBase::ArtifactCollected()
{
    CollectedArtifactsCount++;
	
	ARoverVehiclePawn* Rover = Cast<ARoverVehiclePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	Rover->SetCollectedArtifactCount(CollectedArtifactsCount);
	CheckWinCondition();
}

int32 AVehicleStarterGameModeBase::GetCollectedArtifactsCount() const
{
    return CollectedArtifactsCount;
}

void AVehicleStarterGameModeBase::AddPhotoTaken(TArray<FString> PhotoName)
{
	//for each photo taken, add the name to the array if not already present
	for (FString Name : PhotoName)
	{
		if (!PhotosTaken.Contains(Name))
		{
			PhotosTaken.Add(Name);
		}
	}

	PhotosTakenCount = PhotosTaken.Num();
	ARoverVehiclePawn* Rover = Cast<ARoverVehiclePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	Rover->SetPhotoCount(PhotosTakenCount);
	CheckWinCondition();

}

void AVehicleStarterGameModeBase::CheckWinCondition()
{
	//if the player has collected all artifacts and taken all photos, they win
	if (CollectedArtifactsCount >= 3 && PhotosTakenCount >= 3)
	{
		//Display win message
		UE_LOG(LogTemp, Warning, TEXT("You win!"));
		ARoverVehiclePawn* Rover = Cast<ARoverVehiclePawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Rover->DisplayWinScreen();
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AVehicleStarterGameModeBase::RestartGame, 5.0f, false);

	}
}

void AVehicleStarterGameModeBase::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
