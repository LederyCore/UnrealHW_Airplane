// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "Airplane.h"
#include "AirplanePlayerController.h"

AMyGameMode::AMyGameMode()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 3.f, FColor::Red, TEXT("Hello World"));
	}

	DefaultPawnClass = AAirplane::StaticClass();
	PlayerControllerClass = AAirplanePlayerController::APlayerController::StaticClass();
}
