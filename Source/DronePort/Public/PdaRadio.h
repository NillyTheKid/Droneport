// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PdaRadio.generated.h"

class UAudioComponent;
class UTextRenderComponent;

UCLASS()
class DRONEPORT_API APdaRadio : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APdaRadio();

	UFUNCTION(BlueprintCallable) void RegisterRadioStation(FString stationName, UAudioComponent* stationAudio);
	UFUNCTION(BlueprintCallable) void SetActive(bool newActive);
	UFUNCTION(BlueprintCallable) void GoToNextStation();
	UFUNCTION(BlueprintCallable) void GoToPrevStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

	UFUNCTION(BlueprintCallable) void SetActiveStation(FString stationName);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UAudioComponent* _pEmptyAudioComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UTextRenderComponent* _pCurrentChannelDisp;
private:
	TMap<FString, UAudioComponent*> _RadioStations;
	TArray<FString> _RadioSelectionList;
	int _CurrSelectedRadio;
	FString _CurrentStationName;
	bool _IsActive = false;
};
