// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DroneportGameMode.generated.h"

class AVolocopter;
class AVolocopterPath;
class APlayerBase;
class APolaroid;
class AFlightStickTarget;

/**
 * 
 */
UCLASS()
class DRONEPORT_API ADroneportGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable) void ResetGame();

protected:
	UFUNCTION(BlueprintCallable) void SpawnVolocopter(TSubclassOf<AVolocopterPath> pathClass, TSubclassOf<AVolocopter> volocopterClass, AVolocopter*& pVolocopter);
	UFUNCTION(BlueprintCallable) void SetPlayer(APlayerBase* pPlayer);
	UFUNCTION(BlueprintCallable) void SetPolaroid(APolaroid* pPolaroid);
	UFUNCTION(BlueprintCallable) void SetFlightStickTarget(AFlightStickTarget* pFlightStickTarget);

private:
	AVolocopter* _pVolocopter;
	AVolocopterPath* _pPath;
	APlayerBase* _pPlayer;
	APolaroid* _pPolaroid;
	AFlightStickTarget* _pFlightStickTarget;
};
