
#include "AirplaneMovementComponent.h"

UAirplaneMovementComponent::UAirplaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAirplaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
}