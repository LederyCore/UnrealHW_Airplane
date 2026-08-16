// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundAirplaneMovement.h"
#include "AirplaneMovementComponent.h"
#include "TakingOffAirplaneMovement.h"
#include "Components/PrimitiveComponent.h"

void UGroundAirplaneMovement::Enter(UAirplaneMovementComponent* MovementComponent)
{
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(MovementComponent->UpdatedComponent))
	{
		PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		PrimitiveComponent->SetSimulatePhysics(true);
	}
}

void UGroundAirplaneMovement::Exit(UAirplaneMovementComponent* MovementComponent)
{

}

void UGroundAirplaneMovement::TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime)
{
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(MovementComponent->UpdatedComponent);
	if (!PrimitiveComponent)
	{
		return;
	}

	const float TargetSpeed = MovementComponent->GetThrottleInput() * MaxGroundSpeed;
	const float CurrentSpeed = MovementComponent->GetCurrentSpeed();
	const float RateToUse = (FMath::Abs(TargetSpeed) > FMath::Abs(CurrentSpeed)) ? Acceleration : Deceleration;
	const float NewSpeed = FMath::FInterpConstantTo(CurrentSpeed, TargetSpeed, DeltaTime, RateToUse);
	MovementComponent->SetCurrentSpeed(NewSpeed);

	const FVector Forward = PrimitiveComponent->GetForwardVector();
	const FVector CurrentVelocity = PrimitiveComponent->GetPhysicsLinearVelocity();
	const FVector NewVelocity(Forward.X * NewSpeed, Forward.Y * NewSpeed, CurrentVelocity.Z);
	PrimitiveComponent->SetPhysicsLinearVelocity(NewVelocity);

	const float YawRate = MovementComponent->GetRollInput() * SteeringTurnRate;
	PrimitiveComponent->SetPhysicsAngularVelocityInDegrees(FVector(0.f, 0.f, YawRate));

	if (NewVelocity.Size2D() >= TakeoffSpeedThreshold)
	{
		MovementComponent->TransitionToState<UTakingOffAirplaneMovement>();
	}
}
