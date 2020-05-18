// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PdaButtonBase.h"
#include "Pda_SoundButton.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPORT_API APda_SoundButton : public APdaButtonBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool isVoiceButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool isMuteButton;
};
