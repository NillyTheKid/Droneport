// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PdaSoundApp.generated.h"

class APdaSlider;

UCLASS()
class DRONEPORT_API APdaSoundApp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APdaSoundApp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void SetData(APdaSlider* pMusicSlider, APdaSlider* pVoiceSlider);

	UFUNCTION(BlueprintCallable) float GetMusicVolume();
	UFUNCTION(BlueprintCallable) float GetVoiceVolume();
private:
	APdaSlider* _pMusicSlider;
	APdaSlider* _pVoiceSlider;
};
