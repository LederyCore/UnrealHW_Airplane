// Fill out your copyright notice in the Description page of Project Settings.


#include "RotateStaticMeshComponent.h"

URotateStaticMeshComponent::URotateStaticMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URotateStaticMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bAutoRotate)
	{
		return;
	}

	AddLocalRotation(RotationSpeed * DeltaTime);
}