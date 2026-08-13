#include "AirplaneMovementComponent.h"

#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"

UAirplaneMovementComponent::UAirplaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAirplaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	const FVector Forward = UpdatedComponent->GetForwardVector();
	const float ThrottleInput = ConsumeThrottleInput();
	float ForwardSpeed = FVector::DotProduct(Velocity, Forward);

	if (!FMath::IsNearlyZero(ThrottleInput))
	{
		Velocity += Forward * ThrottleInput * Acceleration * DeltaTime;
	}
	else if (!bAirborne && !FMath::IsNearlyZero(ForwardSpeed))
	{
		const float BrakeAmount = FMath::Min(FMath::Abs(ForwardSpeed), BrakingDeceleration * DeltaTime);
		Velocity -= Forward * FMath::Sign(ForwardSpeed) * BrakeAmount;
	}

	if (bAirborne && !Velocity.IsNearlyZero())
	{
		const FVector ForwardVelocity = Forward * FVector::DotProduct(Velocity, Forward);
		Velocity = FMath::VInterpTo(Velocity, ForwardVelocity, DeltaTime, AirSteeringInterpSpeed);
	}

	ForwardSpeed = FVector::DotProduct(Velocity, Forward);
	const float LiftAlpha = FMath::Clamp(ForwardSpeed / FMath::Max(TakeoffSpeed, 1.0f), 0.0f, 1.0f);
	const FVector Lift = FVector::UpVector * MaxLiftAcceleration * LiftAlpha;
	const float GravityScale = bAirborne ? GlideGravityScale : 1.0f;
	const FVector Gravity = FVector::DownVector * GravityAcceleration * GravityScale;
	Velocity += (Lift + Gravity) * DeltaTime;

	Velocity -= Velocity * FMath::Clamp(LinearDrag * DeltaTime, 0.0f, 1.0f);
	Velocity = Velocity.GetClampedToMaxSize(MaxSpeed);

	const FVector Delta = Velocity * DeltaTime;
	FHitResult Hit;
	SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.IsValidBlockingHit())
	{
		SetAirborne(false);
		SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit);

		const float VelocityIntoSurface = FVector::DotProduct(Velocity, Hit.Normal);
		if (VelocityIntoSurface < 0.0f)
		{
			Velocity -= Hit.Normal * VelocityIntoSurface;
		}
	}
	else
	{
		SetAirborne(true);
	}
}

void UAirplaneMovementComponent::AddThrottleInput(float Value)
{
	PendingThrottleInput += FMath::Clamp(Value, -1.0f, 1.0f);
}

float UAirplaneMovementComponent::GetForwardSpeed() const
{
	if (!UpdatedComponent)
	{
		return 0.0f;
	}

	return FVector::DotProduct(Velocity, UpdatedComponent->GetForwardVector());
}

bool UAirplaneMovementComponent::IsAirborne() const
{
	return bAirborne;
}

void UAirplaneMovementComponent::SetAirborne(bool bNewAirborne)
{
	if (bAirborne == bNewAirborne)
	{
		return;
	}

	bAirborne = bNewAirborne;

	if (!bDisableUpdatedComponentGravityInAir)
	{
		return;
	}

	if (UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(UpdatedComponent))
	{
		Primitive->SetEnableGravity(!bAirborne);
	}
}

float UAirplaneMovementComponent::ConsumeThrottleInput()
{
	const float ThrottleInput = FMath::Clamp(PendingThrottleInput, -1.0f, 1.0f);
	PendingThrottleInput = 0.0f;
	return ThrottleInput;
}
