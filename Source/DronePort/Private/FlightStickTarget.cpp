// Fill out your copyright notice in the Description page of Project Settings.


#include "FlightStickTarget.h"
#include "Components/ArrowComponent.h"

// Sets default values
AFlightStickTarget::AFlightStickTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetSimulatePhysicsOnDrop(false);
}


UFUNCTION(BlueprintCallable) void AFlightStickTarget::ClampPosition()
{
	auto basePos = _pIdlePosComp->K2_GetComponentLocation();
	auto localPos = GetActorLocation() - basePos;

	localPos.X = FMath::Clamp(localPos.X, _MinPosition.X, _MaxPosition.X);
	localPos.Y = FMath::Clamp(localPos.Y, _MinPosition.Y, _MaxPosition.Y);
	localPos.Z = FMath::Clamp(localPos.Z, _MinPosition.Z, _MaxPosition.Z);

	SetActorLocation(basePos + localPos);
}

UFUNCTION()FVector2D AFlightStickTarget::ScaleInput(FVector2D distances)
{
	FVector2D result;

	result.X = ScaleInput(distances.X, _MinPosition.X, _MaxPosition.X);
	result.Y = ScaleInput(distances.Y, _MinPosition.Y, _MaxPosition.Y);

	return result;
}

// Called every frame
void AFlightStickTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlightStickTarget::OnGrab()
{
	_IsGrabbed = true;
}

void AFlightStickTarget::OnRelease()
{
	_IsGrabbed = false;
}

float AFlightStickTarget::ScaleInput(float input, float min, float max)
{
	if (input >= 0)
	{
		return input / max;
	}
	else
	{
		return -(input / min);
	}
}

