// Fill out your copyright notice in the Description page of Project Settings.

#include "TakingOffAirplaneMovement.h"
#include "AirplaneMovementComponent.h"
#include "FlyingAirplaneMovement.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/HitResult.h"

void UTakingOffAirplaneMovement::Enter(UAirplaneMovementComponent* MovementComponent)
{
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(MovementComponent->UpdatedComponent))
	{
		PrimitiveComponent->SetSimulatePhysics(false);
	}

	StartAltitude = 0.f;
	if (const USceneComponent* UpdatedComponent = MovementComponent->UpdatedComponent)
	{
		StartAltitude = UpdatedComponent->GetComponentLocation().Z;
	}
}

void UTakingOffAirplaneMovement::Exit(UAirplaneMovementComponent* MovementComponent)
{

}

void UTakingOffAirplaneMovement::TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime)
{
	USceneComponent* UpdatedComponent = MovementComponent->UpdatedComponent;
	if (!UpdatedComponent)
	{
		return;
	}

	float NewSpeed = MovementComponent->GetCurrentSpeed();
	const float ThrottleInput = MovementComponent->GetThrottleInput();
	if (ThrottleInput > 0.f)
	{
		const float TargetSpeed = ThrottleInput * MaxAirSpeed;
		const float RateToUse = (TargetSpeed > NewSpeed) ? Acceleration : Deceleration;
		NewSpeed = FMath::FInterpConstantTo(NewSpeed, TargetSpeed, DeltaTime, RateToUse);
	}
	else if (ThrottleInput < 0.f)
	{
		NewSpeed = FMath::FInterpConstantTo(NewSpeed, 0.f, DeltaTime, Deceleration);
	}
	MovementComponent->SetCurrentSpeed(NewSpeed);

	const FRotator CurrentRotation = UpdatedComponent->GetComponentRotation();
	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Pitch = ClimbPitchTarget;
	TargetRotation.Roll += MovementComponent->GetRollInput() * RollRate * DeltaTime;

	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);

	const FVector Delta = NewRotation.Vector() * NewSpeed * DeltaTime;

	FHitResult Hit;
	MovementComponent->SafeMoveUpdatedComponent(Delta, NewRotation.Quaternion(), true, Hit);

	const float AltitudeGained = UpdatedComponent->GetComponentLocation().Z - StartAltitude;
	if (AltitudeGained >= ClimbCompleteAltitude)
	{
		MovementComponent->TransitionToState<UFlyingAirplaneMovement>();
	}
}
