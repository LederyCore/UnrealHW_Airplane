// Fill out your copyright notice in the Description page of Project Settings.


#include "AirplanePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Airplane.h"

AAirplanePlayerController::AAirplanePlayerController()
{
	bShowMouseCursor = false;
}

void AAirplanePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem
			= LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	CacheControlledAirplane();
}

void AAirplanePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInput->BindAction(PitchAction, ETriggerEvent::Triggered, this, &AAirplanePlayerController::HandlePitch);
	EnhancedInput->BindAction(PitchAction, ETriggerEvent::Completed, this, &AAirplanePlayerController::HandlePitchReleased);
	EnhancedInput->BindAction(PitchAction, ETriggerEvent::Canceled, this, &AAirplanePlayerController::HandlePitchReleased);
	EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &AAirplanePlayerController::HandleRoll);
	EnhancedInput->BindAction(RollAction, ETriggerEvent::Completed, this, &AAirplanePlayerController::HandleRollReleased);
	EnhancedInput->BindAction(RollAction, ETriggerEvent::Canceled, this, &AAirplanePlayerController::HandleRollReleased);
	EnhancedInput->BindAction(YawAction, ETriggerEvent::Triggered, this, &AAirplanePlayerController::HandleYaw);
	EnhancedInput->BindAction(YawAction, ETriggerEvent::Completed, this, &AAirplanePlayerController::HandleYawReleased);
	EnhancedInput->BindAction(YawAction, ETriggerEvent::Canceled, this, &AAirplanePlayerController::HandleYawReleased);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AAirplanePlayerController::HandleThrottle);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AAirplanePlayerController::HandleThrottleReleased);
	EnhancedInput->BindAction(ThrottleAction, ETriggerEvent::Canceled, this, &AAirplanePlayerController::HandleThrottleReleased);
	EnhancedInput->BindAction(LandingAction, ETriggerEvent::Started, this, &AAirplanePlayerController::HandleLanding);
}

void AAirplanePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CacheControlledAirplane();
}

void AAirplanePlayerController::CacheControlledAirplane()
{
	CachedAirplane = Cast<AAirplane>(GetPawn());
}

void AAirplanePlayerController::HandlePitch(const FInputActionValue& Value)
{
	if (CachedAirplane)
	{
		CachedAirplane->SetPitchInput(Value.Get<float>());
	}
}

void AAirplanePlayerController::HandlePitchReleased()
{
	if (CachedAirplane)
	{
		CachedAirplane->SetPitchInput(0.f);
	}
}

void AAirplanePlayerController::HandleRoll(const FInputActionValue& Value)
{
	if (CachedAirplane)
	{
		CachedAirplane->SetRollInput(Value.Get<float>());
	}
}

void AAirplanePlayerController::HandleRollReleased()
{
	if (CachedAirplane)
	{
		CachedAirplane->SetRollInput(0.f);
	}
}

void AAirplanePlayerController::HandleYaw(const FInputActionValue& Value)
{
	if (CachedAirplane)
	{
		CachedAirplane->SetYawInput(Value.Get<float>());
	}
}

void AAirplanePlayerController::HandleYawReleased()
{
	if (CachedAirplane)
	{
		CachedAirplane->SetYawInput(0.f);
	}
}

void AAirplanePlayerController::HandleThrottle(const FInputActionValue& Value)
{
	if (CachedAirplane)
	{
		CachedAirplane->SetThrottleInput(Value.Get<float>());
	}
}

void AAirplanePlayerController::HandleThrottleReleased()
{
	if (CachedAirplane)
	{
		CachedAirplane->SetThrottleInput(0.f);
	}
}

void AAirplanePlayerController::HandleLanding()
{
	if (CachedAirplane)
	{
		CachedAirplane->ToggleLandingMode();
	}
}
