// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PdaSlider.generated.h"

class UBoxComponent;
class UDecalComponent;

UCLASS()
class DRONEPORT_API APdaSlider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APdaSlider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) float GetSliderPercentage();
	UFUNCTION() void Reset();
private:
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx, bool isFromSweep, const FHitResult& sweepResult);
	UFUNCTION()	void OnOverlapEnd(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FVector colliderDims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	UMaterialInterface* pSliderDecalMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	UMaterialInterface* pBarDecalMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector sliderDecalSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float barWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float barHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float startValue;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UBoxComponent* _pColliderComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UDecalComponent* _pSliderDecalComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UDecalComponent* _pBarDecalComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) USceneComponent* _pMovableComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) USceneComponent* _pBarLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) USceneComponent* _pBarRight;

private:
	UBoxComponent* _pFingerComp;

	float _BarWidthDist;
};
