// Fill out your copyright notice in the Description page of Project Settings.


#include "Pda_AppButton.h"
#include "PdaBase.h"

UFUNCTION(BlueprintCallable) void APda_AppButton::SetData(FString appName, APdaBase* pPda)
{
	_AppName = appName;
	_pPda = pPda;
}

void APda_AppButton::OnPress()
{
	_pPda->SetActiveApp(_AppName);
}

void APda_AppButton::OnRelease()
{
}