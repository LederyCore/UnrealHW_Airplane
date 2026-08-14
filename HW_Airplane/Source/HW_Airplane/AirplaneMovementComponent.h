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

	void SetPitchInput(float Value);
	void SetRollInput(float Value);
	void SetYawInput(float Value);
	void SetThrottleInput(float Value);
	void ToggleLandingMode();

private:

};
