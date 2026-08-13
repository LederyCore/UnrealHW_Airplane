#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AirplaneMovementComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Movement), meta=(BlueprintSpawnableComponent, DisplayName="Airplane Movement"))
class HW_AIRPLANE_API UAirplaneMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UAirplaneMovementComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Airplane|Movement")
	void AddThrottleInput(float Value);

	UFUNCTION(BlueprintPure, Category = "Airplane|Movement")
	float GetForwardSpeed() const;

	UFUNCTION(BlueprintPure, Category = "Airplane|Movement")
	bool IsAirborne() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Movement", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Movement", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Acceleration = 1800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Movement", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BrakingDeceleration = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Flight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float TakeoffSpeed = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Flight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxLiftAcceleration = 1400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Flight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float GravityAcceleration = 980.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Flight", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
	float GlideGravityScale = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Flight", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AirSteeringInterpSpeed = 1.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Flight")
	bool bDisableUpdatedComponentGravityInAir = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Airplane|Movement", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float LinearDrag = 0.15f;

private:
	float ConsumeThrottleInput();
	void SetAirborne(bool bNewAirborne);

	float PendingThrottleInput = 0.0f;
	bool bAirborne = false;
};
