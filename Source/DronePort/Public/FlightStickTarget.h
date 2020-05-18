// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickupable.h"
#include "FlightStickTarget.generated.h"


UCLASS()
class DRONEPORT_API AFlightStickTarget : public APickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlightStickTarget();

	UFUNCTION(BlueprintCallable) void ClampPosition();

	UFUNCTION() FVector2D ScaleInput(FVector2D distances);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnGrab() override;
	virtual void OnRelease() override;

private:
	float ScaleInput(float input, float min, float max);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) bool _IsGrabbed;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) FVector _MinPosition;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) FVector _MaxPosition;

private:
};
