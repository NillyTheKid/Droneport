// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaClockDigital.h"
#include "Components/TextRenderComponent.h"

// Sets default values
APdaClockDigital::APdaClockDigital()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Default Root");
	_pTextComp = CreateDefaultSubobject<UTextRenderComponent>("Text");

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pTextComp->AttachToComponent(RootComponent, rules);
}

// Called when the game starts or when spawned
void APdaClockDigital::BeginPlay()
{
	Super::BeginPlay();
	
}

void APdaClockDigital::OnConstruction(const FTransform& transform)
{
	_pTextComp->SetFont(_pFont);
	_pTextComp->SetTextMaterial(_pTextMat);
	_pTextComp->SetHorizontalAlignment(_HorizontalAlignment);
	_pTextComp->SetVerticalAlignment(_VerticalAlignment);
	_pTextComp->SetTextRenderColor(_TextColor);
	_pTextComp->SetWorldSize(_TextSize);
	_pTextComp->SetXScale(1.f);
	_pTextComp->SetYScale(1.f);
}

// Called every frame
void APdaClockDigital::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FString text = FDateTime::Now().GetTimeOfDay().ToString();
	text = text.Mid(1, 8);
	_pTextComp->SetText(text);
}

