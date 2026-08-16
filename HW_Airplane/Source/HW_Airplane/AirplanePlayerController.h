// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AirplanePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class AAirplane;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class HW_AIRPLANE_API AAirplanePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :
	AAirplanePlayerController();

protected :
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private :
	void CacheControlledAirplane();
	void HandlePitch(const FInputActionValue& Value);
	void HandlePitchReleased();
	void HandleRoll(const FInputActionValue& Value);
	void HandleRollReleased();
	void HandleYaw(const FInputActionValue& Value);
	void HandleYawReleased();
	void HandleThrottle(const FInputActionValue& Value);
	void HandleThrottleReleased();
	void HandleLanding();

private :
	UPROPERTY()
	TObjectPtr<AAirplane> CachedAirplane;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> PitchAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> YawAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ThrottleAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LandingAction;
};
