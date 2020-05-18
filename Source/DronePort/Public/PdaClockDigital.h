// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PdaClockDigital.generated.h"

class UTextRenderComponent;

UCLASS()
class DRONEPORT_API APdaClockDigital : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APdaClockDigital();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UFont* _pFont;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UMaterialInterface* _pTextMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<enum EHorizTextAligment> _HorizontalAlignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TEnumAsByte<enum EVerticalTextAligment> _VerticalAlignment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FColor _TextColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float _TextSize;

private:
	UPROPERTY(VisibleDefaultsOnly) UTextRenderComponent* _pTextComp;
};
