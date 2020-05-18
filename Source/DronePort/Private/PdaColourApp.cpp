// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaColourApp.h"
#include "PdaSlider.h"

// Sets default values
APdaColourApp::APdaColourApp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APdaColourApp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APdaColourApp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UFUNCTION(BlueprintCallable) void APdaColourApp::GetColour(FLinearColor& colour)
{
	colour = _Colour;
}

void APdaColourApp::SetColour(FLinearColor colour)
{
	_Colour = colour;
}



UFUNCTION() void APdaColourApp::Reset()
{
	_Colour = FLinearColor(1, 1, 1);
}