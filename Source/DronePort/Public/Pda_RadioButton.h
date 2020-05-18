// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PdaButtonBase.h"
#include "Pda_RadioButton.generated.h"

class APdaRadio;

/**
 * 
 */
UCLASS()
class DRONEPORT_API APda_RadioButton : public APdaButtonBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable) void SetData(APdaRadio* radio, bool isNextButton);

protected:
	virtual void OnPress() override;
	virtual void OnRelease() override;

private:
	APdaRadio* _pRadio;
	bool _IsNextButton;
};
