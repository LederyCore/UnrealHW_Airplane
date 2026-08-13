// Fill out your copyright notice in the Description page of Project Settings.


#include "Airplane.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AirplaneMovementComponent.h"
#include "AirplanePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AAirplane::AAirplane()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

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

// Called when the game starts or when spawned
void AAirplane::BeginPlay()
{
	Super::BeginPlay();
	BindInput();
}

// Called every frame
void AAirplane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyFlightControls(DeltaTime);
}

// Called to bind functionality to input
void AAirplane::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent =
		CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (LookAction)
	{
		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&AAirplane::Look
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Completed,
			this,
			&AAirplane::StopLook
		);

		EnhancedInputComponent->BindAction(
			LookAction,
			ETriggerEvent::Canceled,
			this,
			&AAirplane::StopLook
		);
	}

	if (RollAction)
	{
		EnhancedInputComponent->BindAction(
			RollAction,
			ETriggerEvent::Triggered,
			this,
			&AAirplane::Roll
		);

		EnhancedInputComponent->BindAction(
			RollAction,
			ETriggerEvent::Completed,
			this,
			&AAirplane::StopRoll
		);

		EnhancedInputComponent->BindAction(
			RollAction,
			ETriggerEvent::Canceled,
			this,
			&AAirplane::StopRoll
		);
	}

	if (ThrottleAction)
	{
		EnhancedInputComponent->BindAction(
			ThrottleAction,
			ETriggerEvent::Triggered,
			this,
			&AAirplane::Throttle
		);
	}
}

UPawnMovementComponent* AAirplane::GetMovementComponent() const
{
	return Movement;
}

void AAirplane::BindInput()
{
	AAirplanePlayerController* PlayerController = Cast<AAirplanePlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer()
		);

		if (SubSystem && DefaultMappingContext)
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAirplane::Look(const FInputActionValue& Value)
{
	TargetLookInput = Value.Get<FVector2D>();
}

void AAirplane::Roll(const FInputActionValue& Value)
{
	TargetRollInput = Value.Get<float>();
}

void AAirplane::StopLook(const FInputActionValue& Value)
{
	TargetLookInput = FVector2D::ZeroVector;
}

void AAirplane::StopRoll(const FInputActionValue& Value)
{
	TargetRollInput = 0.0f;
}

void AAirplane::Throttle(const FInputActionValue& Value)
{
	const float ThrottleValue = Value.Get<float>();

	if (Movement)
	{
		Movement->AddThrottleInput(ThrottleValue);
	}
}

void AAirplane::ApplyFlightControls(float DeltaTime)
{
	const bool bCanControlFlight = Movement && Movement->IsAirborne();
	const FVector2D DesiredLookInput = bCanControlFlight ? TargetLookInput : FVector2D::ZeroVector;
	const float DesiredRollInput = bCanControlFlight ? TargetRollInput : 0.0f;

	CurrentLookInput.X = FMath::FInterpTo(CurrentLookInput.X, DesiredLookInput.X, DeltaTime, FlightControlInterpSpeed);
	CurrentLookInput.Y = FMath::FInterpTo(CurrentLookInput.Y, DesiredLookInput.Y, DeltaTime, FlightControlInterpSpeed);
	CurrentRollInput = FMath::FInterpTo(CurrentRollInput, DesiredRollInput, DeltaTime, FlightControlInterpSpeed);

	if (!bCanControlFlight)
	{
		return;
	}

	AddControllerYawInput(CurrentLookInput.X * YawRate * DeltaTime);
	AddControllerPitchInput(CurrentLookInput.Y * PitchRate * DeltaTime);
	AddControllerRollInput(CurrentRollInput * RollRate * DeltaTime);
}
