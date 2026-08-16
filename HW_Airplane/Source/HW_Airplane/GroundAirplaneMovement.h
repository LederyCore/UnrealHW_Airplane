// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IAirplaneMovement.h"
#include "GroundAirplaneMovement.generated.h"

class UAirplaneMovementComponent;
/*
 *
 */
UCLASS()
class HW_AIRPLANE_API UGroundAirplaneMovement : public UObject, public IIAirplaneMovement
{
	GENERATED_BODY()

public :
	void Enter(UAirplaneMovementComponent* MovementComponent) override;
	void Exit(UAirplaneMovementComponent* MovementComponent) override;
	void TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float MaxGroundSpeed = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float Acceleration = 400.f;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float Deceleration = 600.f;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float SteeringTurnRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float TakeoffSpeedThreshold = 900.f;
};
