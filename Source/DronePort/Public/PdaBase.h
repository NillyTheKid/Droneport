// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PdaBase.generated.h"

class APdaColourApp;

class UChildActorComponent;

UCLASS()
class DRONEPORT_API APdaBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APdaBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

	UFUNCTION(BlueprintCallable) void RegisterApp(FString appName, USceneComponent* pAppRoot);
	UFUNCTION(BlueprintCallable) void SetAppButtonData(UChildActorComponent* pComp, FString appName);
	UFUNCTION(BlueprintCallable) void SetRadioData(UChildActorComponent* pRadioComp,
		UChildActorComponent* pButtonPrevComp, UChildActorComponent* pButtonNextComp);
	UFUNCTION(BlueprintCallable) void SetColourApp(UChildActorComponent* pColourAppComp);
	UFUNCTION(BlueprintCallable) void RegisterColourAppButton(UChildActorComponent* pColourAppComp, UChildActorComponent* pButton);
	UFUNCTION(BlueprintCallable) void SetVolumeAppData(UChildActorComponent* pVolumeAppComp, UChildActorComponent* pVoiceSliderComp, UChildActorComponent* pMusicSliderComp);
	UFUNCTION(BlueprintCallable) float GetMusicVolume(UChildActorComponent* pVolumeAppComp);

	UFUNCTION(BlueprintCallable) void UpdateVoiceVolume(UChildActorComponent* pVolumeAppComp);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void SetActiveApp(FString appName);
	UFUNCTION() APdaColourApp* GetColourApp();

	UFUNCTION(BlueprintCallable) void SetPdaActive(bool isActive);
	UFUNCTION(BlueprintCallable) float GetVoiceVolume();

private:
	TMap<FString, USceneComponent*> _Apps; //Note(Niels): Might wanna look into making custom base class for apps
	FString _ActiveAppName;
	FString _DefaultAppName;

	APdaColourApp* _pColourApp;

	float _VoiceVolume;
};
