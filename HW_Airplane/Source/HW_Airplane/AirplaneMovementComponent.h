#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AirplaneMovementComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Movement), meta=(BlueprintSpawnableComponent, DisplayName="Airplane Movement"))
class HW_AIRPLANE_API UAirplaneMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public :
	UAirplaneMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetPitchInput(float Value);
	void SetRollInput(float Value);
	void SetYawInput(float Value);
	void SetThrottleInput(float Value);
	void ToggleLandingMode();
	bool IsNearGround() const { return bIsNearGround; }
	bool IsGrounded() const { return bIsGrounded; }

private :
	void CheckGround();
	void MoveOnGround(float DeltaTime);

private :
	UPROPERTY(EditAnywhere, Category = "Ground Check")
	float GroundCheckDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	float GroundedDistance = 80.f;

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	TEnumAsByte<ECollisionChannel> GroundTraceChannel = ECC_WorldStatic;

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	FVector GroundCheckBoxExtent = FVector(100.f, 100.f, 10.f);

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	bool bDrawGroundCheck = true;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float GroundMaxSpeed = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float GroundAcceleration = 800.f;

	UPROPERTY(EditAnywhere, Category = "Ground Movement")
	float GroundFriction = 600.f;

	float ThrottleInput = 0.f;
	float CurrentGroundSpeed = 0.f;
	bool bHadGroundHit = false;
	bool bIsNearGround = false;
	bool bIsGrounded = false;
	FHitResult GroundHit;
};
