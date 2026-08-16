// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingAirplaneMovement.h"
#include "AirplaneMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/HitResult.h"

void UFlyingAirplaneMovement::Enter(UAirplaneMovementComponent* MovementComponent)
{
	if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(MovementComponent->UpdatedComponent))
	{
		PrimitiveComponent->SetSimulatePhysics(false);
	}

	if (USceneComponent* UpdatedComponent = MovementComponent->UpdatedComponent)
	{
		FRotator LevelRotation = UpdatedComponent->GetComponentRotation();
		LevelRotation.Pitch = 0.f;
		UpdatedComponent->SetWorldRotation(LevelRotation);
	}
}

void UFlyingAirplaneMovement::Exit(UAirplaneMovementComponent* MovementComponent)
{

}

void UFlyingAirplaneMovement::TickMovement(UAirplaneMovementComponent* MovementComponent, float DeltaTime)
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
	TargetRotation.Pitch += MovementComponent->GetPitchInput() * PitchRate * DeltaTime;
	TargetRotation.Roll += MovementComponent->GetRollInput() * RollRate * DeltaTime;
	TargetRotation.Yaw += MovementComponent->GetYawInput() * YawRate * DeltaTime;

	const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);

	const FVector Delta = NewRotation.Vector() * NewSpeed * DeltaTime;

	FHitResult Hit;
	MovementComponent->SafeMoveUpdatedComponent(Delta, NewRotation.Quaternion(), true, Hit);
}
