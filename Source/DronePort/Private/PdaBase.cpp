// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaBase.h"
#include "Components/ChildActorComponent.h"
#include "Pda_Appbutton.h"
#include "Kismet/GameplayStatics.h"
#include "Pda_RadioButton.h"
#include "PdaRadio.h"
#include "PdaColourApp.h"
#include "PdaSlider.h"
#include "Pda_ColourButton.h"
#include "PdaSoundApp.h"

// Sets default values
APdaBase::APdaBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APdaBase::BeginPlay()
{
	Super::BeginPlay();

	_DefaultAppName = _ActiveAppName;
	
}

void APdaBase::OnConstruction(const FTransform& transform)
{
	for (auto app : _Apps)
	{
		app.Value->SetActive(false);
		app.Value->SetVisibility(false, true);
		
		TArray<USceneComponent*> children;
		app.Value->GetChildrenComponents(true, children);
		for (auto child : children)
		{
			child->SetActive(false);
			auto comp = Cast<UChildActorComponent>(child);

			if (IsValid(comp))
			{
				comp->GetChildActor()->SetActorEnableCollision(false);
			}
		}
	}

	if (_Apps.Find("Home") != nullptr)
	{
		_Apps["Home"]->SetActive(true);
		_Apps["Home"]->SetVisibility(true, true);
		_ActiveAppName = "Home";

		TArray<USceneComponent*> children;
		_Apps["Home"]->GetChildrenComponents(true, children);
		for (auto child : children)
		{
			child->SetActive(true);
			auto comp = Cast<UChildActorComponent>(child);

			if (IsValid(comp))
			{
				comp->GetChildActor()->SetActorEnableCollision(true);
			}
		}
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "ERROR: No 'Home' App found");
	}
}

UFUNCTION(BlueprintCallable) void APdaBase::RegisterApp(FString appName, USceneComponent* pAppRoot)
{
	_Apps.Emplace(appName, pAppRoot);
}

UFUNCTION(BlueprintCallable) void APdaBase::SetAppButtonData(UChildActorComponent* pComp, FString appName)
{
	((APda_AppButton*)pComp->GetChildActor())->SetData(appName, this);
}

UFUNCTION(BlueprintCallable) void APdaBase::SetRadioData(UChildActorComponent* pRadioComp,
	UChildActorComponent* pButtonPrevComp, UChildActorComponent* pButtonNextComp)
{
	auto radio = ((APdaRadio*)pRadioComp->GetChildActor());
	auto buttonPrev = ((APda_RadioButton*)pButtonPrevComp->GetChildActor());
	auto buttonNext = ((APda_RadioButton*)pButtonNextComp->GetChildActor());

	buttonPrev->SetData(radio, false);
	buttonNext->SetData(radio, true);
}

UFUNCTION(BlueprintCallable) void APdaBase::SetColourApp(UChildActorComponent* pColourAppComp)
{
	_pColourApp = ((APdaColourApp*)pColourAppComp->GetChildActor());
}

UFUNCTION(BlueprintCallable) void APdaBase::RegisterColourAppButton(UChildActorComponent* pColourAppComp, UChildActorComponent* pButton)
{
	((APda_ColourButton*)pButton->GetChildActor())->SetApp(((APdaColourApp*)pColourAppComp->GetChildActor()));
}

UFUNCTION(BlueprintCallable) void APdaBase::SetVolumeAppData(UChildActorComponent* pVolumeAppComp, UChildActorComponent* pVoiceSliderComp, UChildActorComponent* pMusicSliderComp)
{
	APdaSlider* voice = (APdaSlider*)pVoiceSliderComp->GetChildActor();
	APdaSlider* music = (APdaSlider*)pMusicSliderComp->GetChildActor();
	((APdaSoundApp*)pVolumeAppComp->GetChildActor())->SetData(music, voice);
}

UFUNCTION(BlueprintCallable) float APdaBase::GetMusicVolume(UChildActorComponent* pVolumeAppComp)
{
	return ((APdaSoundApp*)pVolumeAppComp->GetChildActor())->GetMusicVolume();
}

UFUNCTION(BlueprintCallable) void APdaBase::UpdateVoiceVolume(UChildActorComponent* pVolumeAppComp)
{
	_VoiceVolume = ((APdaSoundApp*)pVolumeAppComp->GetChildActor())->GetVoiceVolume();
}

// Called every frame
void APdaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UFUNCTION(BlueprintCallable) void APdaBase::SetActiveApp(FString appName)
{
	TArray<USceneComponent*> children;

	_Apps[_ActiveAppName]->SetActive(false);
	_Apps[_ActiveAppName]->SetVisibility(false, true);
	_Apps[_ActiveAppName]->GetChildrenComponents(true, children);
	for (auto child : children)
	{
		child->SetActive(false);
		auto comp = Cast<UChildActorComponent>(child);

		if (IsValid(comp))
		{
			comp->GetChildActor()->SetActorEnableCollision(false);
		}
	}

	_Apps[appName]->SetActive(true);
	_Apps[appName]->SetVisibility(true, true);
	_Apps[appName]->GetChildrenComponents(true, children);
	for (auto child : children)
	{
		child->SetActive(true);
		auto comp = Cast<UChildActorComponent>(child);

		if (IsValid(comp))
		{
			comp->GetChildActor()->SetActorEnableCollision(true);
		}
	}

	_ActiveAppName = appName;
}

UFUNCTION()APdaColourApp* APdaBase::GetColourApp()
{
	return _pColourApp;
}

UFUNCTION(BlueprintCallable) void APdaBase::SetPdaActive(bool isActive)
{
	if (isActive)
	{
		_ActiveAppName = _DefaultAppName;
		
		TArray<USceneComponent*> children;

		_Apps[_DefaultAppName]->SetActive(true);
		_Apps[_DefaultAppName]->SetVisibility(true, true);
		_Apps[_DefaultAppName]->GetChildrenComponents(true, children);
		for (auto child : children)
		{
			child->SetActive(true);
			auto comp = Cast<UChildActorComponent>(child);

			if (IsValid(comp))
			{
				comp->GetChildActor()->SetActorEnableCollision(true);
			}
		}
	}
	else
	{
		TArray<USceneComponent*> children;

		_Apps[_ActiveAppName]->SetActive(false);
		_Apps[_ActiveAppName]->SetVisibility(false, true);
		_Apps[_ActiveAppName]->GetChildrenComponents(true, children);
		for (auto child : children)
		{
			child->SetActive(false);
			auto comp = Cast<UChildActorComponent>(child);

			if (IsValid(comp))
			{
				comp->GetChildActor()->SetActorEnableCollision(false);
			}
		}
	}
}

UFUNCTION(BlueprintCallable) float APdaBase::GetVoiceVolume()
{
	return _VoiceVolume;
}

