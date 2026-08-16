// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IAirplaneMovement.h"
#include "FlyingAirplaneMovement.generated.h"

class UAirplaneMovementComponent;
/*
 *
 */
UCLASS()
class HW_AIRPLANE_API UFlyingAirplaneMovement : public UObject, public IIAirplaneMovement
{
	GENERATED_BODY()

public :
	void Enter(UAirplaneMovementComponent* MovementComponent) override;
	void Exit(UAirplaneMovementComponent* MovementComponent) override;
	void TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float MaxAirSpeed = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float Acceleration = 300.f;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float Deceleration = 300.f;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float RotationInterpSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float PitchRate = 60.f;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float RollRate = 90.f;

	UPROPERTY(EditAnywhere, Category = "Flying")
	float YawRate = 45.f;
};
