// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PdaButtonBase.h"
#include "Pda_ColourButton.generated.h"

/**
 * 
 */
class APdaColourApp;

UCLASS()
class DRONEPORT_API APda_ColourButton : public APdaButtonBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FLinearColor colour;

	UFUNCTION(BlueprintCallable) void SetApp(APdaColourApp* pApp);

protected:
	virtual void OnPress() override;
	virtual void OnRelease() override;

private:
	APdaColourApp* _pColourApp;
};
