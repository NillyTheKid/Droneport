// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaRadio.h"
#include "Components/AudioComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
APdaRadio::APdaRadio()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Default Root");
	_pEmptyAudioComp = CreateDefaultSubobject<UAudioComponent>("ChannelNone");
	_pCurrentChannelDisp = CreateDefaultSubobject<UTextRenderComponent>("Current Channel Display");

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pEmptyAudioComp->AttachToComponent(RootComponent, rules);
	_pCurrentChannelDisp->AttachToComponent(RootComponent, rules);
}

UFUNCTION(BlueprintCallable) void APdaRadio::RegisterRadioStation(FString stationName, UAudioComponent* stationAudio)
{
	_RadioStations.Emplace(stationName, stationAudio);
	_RadioSelectionList.Emplace(stationName);
}

UFUNCTION(BlueprintCallable) void APdaRadio::SetActive(bool newActive)
{
	_IsActive = newActive;

	if (_IsActive)
	{
		SetActiveStation("None");
	}
	else
	{
		_RadioStations[_CurrentStationName]->SetVolumeMultiplier(0);
	}
}

UFUNCTION(BlueprintCallable) void APdaRadio::GoToNextStation()
{
	_RadioStations[_CurrentStationName]->SetVolumeMultiplier(0);

	++_CurrSelectedRadio %= _RadioSelectionList.Num();
	_CurrentStationName = _RadioSelectionList[_CurrSelectedRadio];
	_RadioStations[_CurrentStationName]->SetVolumeMultiplier(1);
	_pCurrentChannelDisp->SetText(_CurrentStationName);
}

UFUNCTION(BlueprintCallable) void APdaRadio::GoToPrevStation()
{
	_RadioStations[_CurrentStationName]->SetVolumeMultiplier(0);

	--_CurrSelectedRadio;
	if (_CurrSelectedRadio < 0) _CurrSelectedRadio = _RadioSelectionList.Num() - 1;
	_CurrentStationName = _RadioSelectionList[_CurrSelectedRadio];
	_RadioStations[_CurrentStationName]->SetVolumeMultiplier(1);
	_pCurrentChannelDisp->SetText(_CurrentStationName);
}

// Called when the game starts or when spawned
void APdaRadio::BeginPlay()
{
	Super::BeginPlay();
	
}

void APdaRadio::OnConstruction(const FTransform& transform)
{
	RegisterRadioStation("None", _pEmptyAudioComp);
}

UFUNCTION(BlueprintCallable) void APdaRadio::SetActiveStation(FString stationName)
{
	_RadioStations[_CurrentStationName]->SetVolumeMultiplier(0);
	_RadioStations[stationName]->SetVolumeMultiplier(1);
	_CurrentStationName = stationName;

	for (int i = 0; i < _RadioSelectionList.Num(); i++)
	{
		if (stationName == _RadioSelectionList[i])
		{
			_CurrSelectedRadio = i;
			break;
		}
	}

	_pCurrentChannelDisp->SetText(_CurrentStationName);
}

// Called every frame
void APdaRadio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

