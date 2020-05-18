// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PdaButtonBase.generated.h"

class UBoxComponent;
class UDecalComponent;

UCLASS()
class DRONEPORT_API APdaButtonBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APdaButtonBase();

	UFUNCTION(BlueprintCallable) void SetMaterial(UMaterialInterface* pMat);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

	UFUNCTION()	virtual void OnPress();
	UFUNCTION()	virtual void OnRelease();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION() void OnOverlapBegin(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx, bool isFromSweep, const FHitResult& sweepResult);
	UFUNCTION()	void OnOverlapEnd(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	FVector colliderDims;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	UMaterialInterface* pDecalMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector decalSize;

private:
	UMETA(DisplayName = "Collider")	UBoxComponent* _pColliderComp;
	UMETA(DisplayName = "Decal")	UDecalComponent* _pDecalComp;

};
