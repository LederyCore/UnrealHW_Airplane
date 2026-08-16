
#include "AirplaneMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "IAirplaneMovement.h"
#include "GroundAirplaneMovement.h"

UAirplaneMovementComponent::UAirplaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAirplaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentMovementInterface)
	{
		CurrentMovementInterface->TickMovement(this, DeltaTime);
	}
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
	UE_LOG(LogTemp, Log, TEXT("Landing Check"));
}

void UAirplaneMovementComponent::ChangeState(UObject* NewState)
{
	if (CurrentMovementInterface)
	{
		CurrentMovementInterface->Exit(this);
	}

	CurrentStateObject = NewState;
	CurrentMovementInterface = Cast<IIAirplaneMovement>(NewState);

	if (CurrentMovementInterface)
	{
		CurrentMovementInterface->Enter(this);
	}
}

void UAirplaneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(GetOrCreateState<UGroundAirplaneMovement>());
}
