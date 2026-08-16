// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IAirplaneMovement.h"
#include "TakingOffAirplaneMovement.generated.h"

class UAirplaneMovementComponent;
/*
 *
 */
UCLASS()
class HW_AIRPLANE_API UTakingOffAirplaneMovement : public UObject, public IIAirplaneMovement
{
	GENERATED_BODY()

public :
	void Enter(UAirplaneMovementComponent* MovementComponent) override;
	void Exit(UAirplaneMovementComponent* MovementComponent) override;
	void TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float MaxAirSpeed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float Acceleration = 300.f;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float Deceleration = 300.f;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float ClimbPitchTarget = 15.f;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float ClimbCompleteAltitude = 500.f;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float RotationInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Taking Off")
	float RollRate = 60.f;

private :
	float StartAltitude = 0.f;
};
