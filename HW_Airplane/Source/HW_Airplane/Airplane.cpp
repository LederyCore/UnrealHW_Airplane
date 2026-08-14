// Fill out your copyright notice in the Description page of Project Settings.


#include "Airplane.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AirplaneMovementComponent.h"

// Sets default values
AAirplane::AAirplane()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UAirplaneMovementComponent>(TEXT("Movement"));
	Movement->SetUpdatedComponent(Box);
}

// Called every frame
void AAirplane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAirplane::SetPitchInput(float Value)
{
	if (UAirplaneMovementComponent* AirplaneMovement = GetAirplaneMovement())
	{
		AirplaneMovement->SetPitchInput(Value);
	}
}

void AAirplane::SetRollInput(float Value)
{
	if (UAirplaneMovementComponent* AirplaneMovement = GetAirplaneMovement())
	{
		AirplaneMovement->SetRollInput(Value);
	}
}

void AAirplane::SetYawInput(float Value)
{
	if (UAirplaneMovementComponent* AirplaneMovement = GetAirplaneMovement())
	{
		AirplaneMovement->SetYawInput(Value);
	}
}

void AAirplane::SetThrottleInput(float Value)
{
	if (UAirplaneMovementComponent* AirplaneMovement = GetAirplaneMovement())
	{
		AirplaneMovement->SetThrottleInput(Value);
	}
}

void AAirplane::ToggleLandingMode()
{
	if (UAirplaneMovementComponent* AirplaneMovement = GetAirplaneMovement())
	{
		AirplaneMovement->ToggleLandingMode();
	}
}

UAirplaneMovementComponent* AAirplane::GetAirplaneMovement()
{
	if (!Movement || Movement->GetOwner() != this)
	{
		Movement = FindComponentByClass<UAirplaneMovementComponent>();
	}

	if (Movement && Box)
	{
		Movement->SetUpdatedComponent(Box);
	}

	return Movement;
}

// Called when the game starts or when spawned
void AAirplane::BeginPlay()
{
	Super::BeginPlay();

	GetAirplaneMovement();
}
