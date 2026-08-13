// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RotateStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta=(BlueprintSpawnableComponent))
class HW_AIRPLANE_API URotateStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public :
	URotateStaticMeshComponent();

protected :
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public :
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationSpeed = FRotator(0.f, 90.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool bAutoRotate = true;
};
