// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VolocopterPath.generated.h"

class USplineComponent;

UCLASS()
class DRONEPORT_API AVolocopterPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVolocopterPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) void HandleTeleport(float teleportDistance);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) float UpdatePosition(float distance);
	UFUNCTION() void Reset();

	USplineComponent* GetSpline() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float _VolocopterPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float _SplineLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USplineComponent* _pSpline;
};
