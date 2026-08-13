// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Airplane.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UPawnMovementComponent;
class UAirplaneMovementComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class HW_AIRPLANE_API AAirplane : public APawn
{
	GENERATED_BODY()

public:	
	AAirplane();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

protected :
	virtual void BeginPlay() override;
	virtual void Look(const FInputActionValue& Value);
	virtual void Roll(const FInputActionValue& Value);
	virtual void StopLook(const FInputActionValue& Value);
	virtual void StopRoll(const FInputActionValue& Value);
	virtual void Throttle(const FInputActionValue& Value);

private :
	void BindInput();
	void ApplyFlightControls(float DeltaTime);


public :
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UAirplaneMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RollAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ThrottleAction;

private :
	FVector2D TargetLookInput = FVector2D::ZeroVector;
	FVector2D CurrentLookInput = FVector2D::ZeroVector;
	float TargetRollInput = 0.0f;
	float CurrentRollInput = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Airplane|Control", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float FlightControlInterpSpeed = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Airplane|Control", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float YawRate = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Airplane|Control", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PitchRate = 45.0f;

	UPROPERTY(EditAnywhere, Category = "Airplane|Control", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RollRate = 75.0f;

};
