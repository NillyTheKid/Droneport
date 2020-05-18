// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaSoundApp.h"
#include "PdaSlider.h"

// Sets default values
APdaSoundApp::APdaSoundApp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APdaSoundApp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APdaSoundApp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UFUNCTION(BlueprintCallable) void APdaSoundApp::SetData(APdaSlider* pMusicSlider, APdaSlider* pVoiceSlider)
{
	_pMusicSlider = pMusicSlider;
	_pVoiceSlider = pVoiceSlider;
}

UFUNCTION(BlueprintCallable) float APdaSoundApp::GetMusicVolume()
{
	return _pMusicSlider->GetSliderPercentage();
}

UFUNCTION(BlueprintCallable) float APdaSoundApp::GetVoiceVolume()
{
	return _pVoiceSlider->GetSliderPercentage();
}

