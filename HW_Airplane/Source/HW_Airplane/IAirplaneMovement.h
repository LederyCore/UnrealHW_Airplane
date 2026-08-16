// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAirplaneMovement.generated.h"

class UAirplaneMovementComponent;

UINTERFACE(MinimalAPI)
class UIAirplaneMovement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HW_AIRPLANE_API IIAirplaneMovement
{
	GENERATED_BODY()

public:
	virtual void Enter(UAirplaneMovementComponent* MovementComponent) = 0;
	virtual void Exit(UAirplaneMovementComponent* MovementComponent) = 0;
	virtual void TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime) = 0;
};
