// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneportGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Volocopter.h"
#include "VolocopterPath.h"
#include "Engine/World.h"
#include "Polaroid.h"
#include "FlightStickTarget.h"
#include "PlayerBase.h"
#include "Components/SplineComponent.h"

UFUNCTION(BlueprintCallable) void ADroneportGameMode::ResetGame()
{
	if (_pPath != nullptr)
	{
		_pPath->Reset();
	}

	if (_pVolocopter != nullptr)
	{
		_pVolocopter->Destroy();
		_pVolocopter = nullptr;
	}
	if (_pPolaroid != nullptr)
	{
		_pPolaroid->Destroy();
		_pPolaroid = nullptr;
	}
	if (_pFlightStickTarget != nullptr)
	{
		_pFlightStickTarget->Destroy();
		_pFlightStickTarget = nullptr;
	}

	if (_pPlayer != nullptr)
	{
		_pPlayer->SetActorLocation(FVector(10000.0, 10000.0, 10000.0));
		_pPlayer->HandleGameReset();
	}
}


UFUNCTION(BlueprintCallable) void ADroneportGameMode::SpawnVolocopter(TSubclassOf<AVolocopterPath> pathClass, TSubclassOf<AVolocopter> volocopterClass, AVolocopter*& pVolocopter)
{
	TArray<AActor*> paths;
	auto world = GetWorld();
	UGameplayStatics::GetAllActorsOfClass(world, pathClass, paths);
	if (paths.Num() == 1)
	{
		//Spawn volocopter and whatnot
		pVolocopter = (AVolocopter*)world->SpawnActor(volocopterClass, new FTransform());
		pVolocopter->SetPath((AVolocopterPath*)paths[0]);

		_pVolocopter = pVolocopter;
		_pPath = (AVolocopterPath*)paths[0];
	}
	else
	{
		UKismetSystemLibrary::PrintString(world, "ERROR: Make sure there is exactly one BP_VolocopterPath actor in the scene!", true, true, FLinearColor(1.f, 0.f, 0.f, 1.f), 5.0f);
		UKismetSystemLibrary::QuitGame(world, nullptr, EQuitPreference::Quit, false);
	}
}

UFUNCTION(BlueprintCallable) void ADroneportGameMode::SetPlayer(APlayerBase* pPlayer)
{
	_pPlayer = pPlayer;
}

UFUNCTION(BlueprintCallable) void ADroneportGameMode::SetPolaroid(APolaroid* pPolaroid)
{
	_pPolaroid = pPolaroid;
}

UFUNCTION(BlueprintCallable) void ADroneportGameMode::SetFlightStickTarget(AFlightStickTarget* pFlightStickTarget)
{
	_pFlightStickTarget = pFlightStickTarget;
}
