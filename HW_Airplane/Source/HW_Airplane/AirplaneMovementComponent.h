#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AirplaneMovementComponent.generated.h"

class IIAirplaneMovement;

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Movement), meta = (BlueprintSpawnableComponent, DisplayName = "Airplane Movement"))
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
	void ChangeState(UObject* NewState);

protected:
	virtual void BeginPlay() override;

private:
	template<typename T>
	T* GetOrCreateState()
	{
		if (TObjectPtr<UObject>* Found = StateCache.Find(T::StaticClass()))
		{
			return Cast<T>(*Found);
		}
		T* NewState = NewObject<T>(this);
		StateCache.Add(T::StaticClass(), NewState);
		return NewState;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airplane Movement", meta = (DisplayName = "Current State"))
	TObjectPtr<UObject> CurrentStateObject;

private:

	UPROPERTY()
	TMap<TSubclassOf<UObject>, TObjectPtr<UObject>> StateCache;

	IIAirplaneMovement* CurrentMovementInterface = nullptr;
	float ThrottleInput = 0.f;
};
