// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PdaButtonBase.h"
#include "Pda_AppButton.generated.h"

class APdaBase;

/**
 * 
 */
UCLASS()
class DRONEPORT_API APda_AppButton : public APdaButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) void SetData(FString appName, APdaBase* pPda);

protected:
	virtual void OnPress() override;
	virtual void OnRelease() override;

private:
	FString _AppName;
	APdaBase* _pPda;
};
