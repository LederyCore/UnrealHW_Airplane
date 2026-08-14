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
	bool IsGrounded() const { return bIsGrounded; }

private :
	void CheckGround();

private :
	UPROPERTY(EditAnywhere, Category = "Ground Check")
	float GroundCheckDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	float GroundedDistance = 80.f;

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	TEnumAsByte<ECollisionChannel> GroundTraceChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	FVector GroundCheckBoxExtent = FVector(10.f, 10.f, 5.f);

	UPROPERTY(EditAnywhere, Category = "Ground Check")
	bool bDrawGroundCheck = true;

	bool bIsGrounded = false;
	FHitResult GroundHit;
};
