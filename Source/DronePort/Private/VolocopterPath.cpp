// Fill out your copyright notice in the Description page of Project Settings.


#include "VolocopterPath.h"
#include "Components/SplineComponent.h"

// Sets default values
AVolocopterPath::AVolocopterPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));
	_pSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
}

// Called when the game starts or when spawned
void AVolocopterPath::BeginPlay()
{
	Super::BeginPlay();
	
	_SplineLength = _pSpline->GetSplineLength();
}

UFUNCTION(BlueprintCallable) void AVolocopterPath::HandleTeleport(float teleportDistance)
{
	_VolocopterPos = FMath::Clamp(_VolocopterPos + teleportDistance, 0.f, _SplineLength);
}

// Called every frame
void AVolocopterPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UFUNCTION(BlueprintCallable) float AVolocopterPath::UpdatePosition(float distance)
{
	_VolocopterPos = FMath::Clamp(_VolocopterPos + distance, 0.f, _SplineLength);
	return _VolocopterPos;
}

UFUNCTION() void AVolocopterPath::Reset()
{
	_VolocopterPos = 0;
}

USplineComponent* AVolocopterPath::GetSpline() const
{
	return _pSpline;
}

