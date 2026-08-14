// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "IAirplaneMovement.h"
#include "GroundAirplaneMovement.generated.h"

class UAirplaneMovementComponent;
/**
 * 
 */
UCLASS()
class HW_AIRPLANE_API UGroundAirplaneMovement : public UObject, public IIAirplaneMovement
{
	GENERATED_BODY()
	
public :
	// IIAirplaneMovement을(를) 통해 상속됨
	void Enter(UAirplaneMovementComponent* MovementComponent) override;
	void Exit(UAirplaneMovementComponent* MovementComponent) override;
	void TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime) override;
};
