
#include "AirplaneMovementComponent.h"
#include "DrawDebugHelpers.h"

UAirplaneMovementComponent::UAirplaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAirplaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckGround();
	MoveOnGround(DeltaTime);
}

void UAirplaneMovementComponent::SetPitchInput(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("Pitch Value: %f"), Value);
}

void UAirplaneMovementComponent::SetRollInput(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("Roll Value: %f"), Value);
}

void UAirplaneMovementComponent::SetYawInput(float Value)
{
	UE_LOG(LogTemp, Log, TEXT("Yaw Value: %f"), Value);
}

void UAirplaneMovementComponent::SetThrottleInput(float Value)
{
	ThrottleInput = FMath::Clamp(Value, -1.f, 1.f);
	UE_LOG(LogTemp, Log, TEXT("Throttle Value: %f"), Value);
}

void UAirplaneMovementComponent::ToggleLandingMode()
{
	UE_LOG(LogTemp, Log, TEXT("NearGround: %s, Grounded: %s"),
		IsNearGround() ? TEXT("true") : TEXT("false"),
		IsGrounded() ? TEXT("true") : TEXT("false"));
}

void UAirplaneMovementComponent::CheckGround()
{
	bHadGroundHit = false;
	bIsNearGround = false;
	bIsGrounded = false;
	GroundHit = FHitResult();

	if (!UpdatedComponent || !GetWorld())
	{
		return;
	}

	const FVector Start = UpdatedComponent->GetComponentLocation();
	const FVector End = Start - FVector::UpVector * GroundCheckDistance;
	const FVector GroundedBoundary = Start - FVector::UpVector * GroundedDistance;
	const FQuat Rotation = UpdatedComponent->GetComponentQuat();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bFindInitialOverlaps = true;

	const bool bHit = GetWorld()->SweepSingleByChannel(
		GroundHit,
		Start,
		End,
		Rotation,
		GroundTraceChannel,
		FCollisionShape::MakeBox(GroundCheckBoxExtent),
		Params
	);
	bHadGroundHit = bHit;

	if (!bHit)
	{
		if (bDrawGroundCheck)
		{
			DrawDebugBox(GetWorld(), Start, GroundCheckBoxExtent, Rotation, FColor::Red, false, 0.f, 0, 2.f);
			DrawDebugBox(GetWorld(), End, GroundCheckBoxExtent, Rotation, FColor::Red, false, 0.f, 0, 2.f);
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.f, 0, 2.f);
			DrawDebugLine(GetWorld(), Start, GroundedBoundary, FColor::Green, false, 0.f, 0, 4.f);
			DrawDebugBox(GetWorld(), GroundedBoundary, GroundCheckBoxExtent, Rotation, FColor::Green, false, 0.f, 0, 2.f);
		}
		return;
	}

	const bool bGroundLikeSurface = GroundHit.ImpactNormal.Z > 0.7f;
	bIsNearGround = bGroundLikeSurface;
	bIsGrounded = GroundHit.Distance <= GroundedDistance && bGroundLikeSurface;

	if (bDrawGroundCheck)
	{
		const FColor DebugColor = bIsGrounded ? FColor::Green : (bIsNearGround ? FColor::Blue : FColor::Yellow);
		DrawDebugBox(GetWorld(), Start, GroundCheckBoxExtent, Rotation, DebugColor, false, 0.f, 0, 2.f);
		DrawDebugBox(GetWorld(), End, GroundCheckBoxExtent, Rotation, DebugColor, false, 0.f, 0, 2.f);
		DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 0.f, 0, 2.f);
		DrawDebugLine(GetWorld(), Start, GroundedBoundary, FColor::Green, false, 0.f, 0, 4.f);
		DrawDebugBox(GetWorld(), GroundedBoundary, GroundCheckBoxExtent, Rotation, FColor::Green, false, 0.f, 0, 2.f);
		DrawDebugPoint(GetWorld(), GroundHit.ImpactPoint, 12.f, DebugColor, false, 0.f);
	}
}

void UAirplaneMovementComponent::MoveOnGround(float DeltaTime)
{
	if (!UpdatedComponent)
	{
		return;
	}

	if (!IsGrounded())
	{
		CurrentGroundSpeed = FMath::FInterpTo(CurrentGroundSpeed, 0.f, DeltaTime, 2.f);
		return;
	}

	if (!FMath::IsNearlyZero(ThrottleInput))
	{
		CurrentGroundSpeed += ThrottleInput * GroundAcceleration * DeltaTime;
		CurrentGroundSpeed = FMath::Clamp(CurrentGroundSpeed, -GroundMaxSpeed, GroundMaxSpeed);
	}
	else
	{
		CurrentGroundSpeed = FMath::FInterpConstantTo(CurrentGroundSpeed, 0.f, DeltaTime, GroundFriction);
	}

	if (FMath::IsNearlyZero(CurrentGroundSpeed))
	{
		return;
	}

	const FVector MoveDelta = UpdatedComponent->GetForwardVector() * CurrentGroundSpeed * DeltaTime;
	FHitResult Hit;
	SafeMoveUpdatedComponent(MoveDelta, UpdatedComponent->GetComponentQuat(), true, Hit);

	if (Hit.IsValidBlockingHit())
	{
		SlideAlongSurface(MoveDelta, 1.f - Hit.Time, Hit.Normal, Hit);
	}
}
