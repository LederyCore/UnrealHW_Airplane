
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
	UE_LOG(LogTemp, Log, TEXT("Throttle Value: %f"), Value);
}

void UAirplaneMovementComponent::ToggleLandingMode()
{
	UE_LOG(LogTemp, Log, TEXT("Toggle Landing Value: %d"), 0);

	if (IsGrounded())
	{
		UE_LOG(LogTemp, Log, TEXT("Airplane is grounded."));
	}
}

void UAirplaneMovementComponent::CheckGround()
{
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

	const bool bHit = GetWorld()->SweepSingleByChannel(
		GroundHit,
		Start,
		End,
		Rotation,
		GroundTraceChannel,
		FCollisionShape::MakeBox(GroundCheckBoxExtent),
		Params
	);

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

	const bool bCloseEnough = GroundHit.Distance <= GroundedDistance;
	const bool bGroundLikeSurface = GroundHit.ImpactNormal.Z > 0.7f;
	bIsGrounded = bCloseEnough && bGroundLikeSurface;

	if (bDrawGroundCheck)
	{
		const FColor DebugColor = bIsGrounded ? FColor::Green : FColor::Yellow;
		DrawDebugBox(GetWorld(), Start, GroundCheckBoxExtent, Rotation, DebugColor, false, 0.f, 0, 2.f);
		DrawDebugBox(GetWorld(), End, GroundCheckBoxExtent, Rotation, DebugColor, false, 0.f, 0, 2.f);
		DrawDebugLine(GetWorld(), Start, End, DebugColor, false, 0.f, 0, 2.f);
		DrawDebugLine(GetWorld(), Start, GroundedBoundary, FColor::Green, false, 0.f, 0, 4.f);
		DrawDebugBox(GetWorld(), GroundedBoundary, GroundCheckBoxExtent, Rotation, FColor::Green, false, 0.f, 0, 2.f);
		DrawDebugPoint(GetWorld(), GroundHit.ImpactPoint, 12.f, DebugColor, false, 0.f);
	}
}
