
#include "AirplaneMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "IAirplaneMovement.h"
#include "GroundAirplaneMovement.h"
#include "TakingOffAirplaneMovement.h"
#include "FlyingAirplaneMovement.h"

UAirplaneMovementComponent::UAirplaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	GroundState = CreateDefaultSubobject<UGroundAirplaneMovement>(TEXT("GroundState"));
	StateCache.Add(UGroundAirplaneMovement::StaticClass(), GroundState);

	TakingOffState = CreateDefaultSubobject<UTakingOffAirplaneMovement>(TEXT("TakingOffState"));
	StateCache.Add(UTakingOffAirplaneMovement::StaticClass(), TakingOffState);

	FlyingState = CreateDefaultSubobject<UFlyingAirplaneMovement>(TEXT("FlyingState"));
	StateCache.Add(UFlyingAirplaneMovement::StaticClass(), FlyingState);
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
	PitchInput = FMath::Clamp(Value, -1.f, 1.f);
}

void UAirplaneMovementComponent::SetRollInput(float Value)
{
	RollInput = FMath::Clamp(Value, -1.f, 1.f);
}

void UAirplaneMovementComponent::SetYawInput(float Value)
{
	YawInput = FMath::Clamp(Value, -1.f, 1.f);
}

void UAirplaneMovementComponent::SetThrottleInput(float Value)
{
	ThrottleInput = FMath::Clamp(Value, -1.f, 1.f);
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

	ChangeState(GroundState);
}
