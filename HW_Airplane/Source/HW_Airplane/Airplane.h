// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Airplane.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UAirplaneMovementComponent;

UCLASS()
class HW_AIRPLANE_API AAirplane : public APawn
{
	GENERATED_BODY()

public:	
	AAirplane();
	virtual void Tick(float DeltaTime) override;

	void SetPitchInput(float Value);
	void SetRollInput(float Value);
	void SetYawInput(float Value);
	void SetThrottleInput(float Value);
	void ToggleLandingMode();

protected :
	virtual void BeginPlay() override;
	

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

private :
	// 랜딩 토글 플래그 필드 (블루프린트 에디터에서 확인은 가능 조작은 불가능.)
};
