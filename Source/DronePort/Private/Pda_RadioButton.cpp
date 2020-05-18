// Fill out your copyright notice in the Description page of Project Settings.


#include "Pda_RadioButton.h"
#include "PdaRadio.h"

UFUNCTION(BlueprintCallable) void APda_RadioButton::SetData(APdaRadio* radio, bool isNextButton)
{
	_pRadio = radio;
	_IsNextButton = isNextButton;
}

void APda_RadioButton::OnPress()
{
	if (_IsNextButton)
	{
		_pRadio->GoToNextStation();
	}
	else
	{
		_pRadio->GoToPrevStation();
	}
}

void APda_RadioButton::OnRelease()
{
}
