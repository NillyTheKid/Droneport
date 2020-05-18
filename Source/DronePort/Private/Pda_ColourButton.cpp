// Fill out your copyright notice in the Description page of Project Settings.


#include "Pda_ColourButton.h"
#include "PdaColourApp.h"

UFUNCTION(BlueprintCallable) void APda_ColourButton::SetApp(APdaColourApp* pApp)
{
	_pColourApp = pApp;
}

void APda_ColourButton::OnPress()
{
	_pColourApp->SetColour(colour);
}

void APda_ColourButton::OnRelease()
{
}
