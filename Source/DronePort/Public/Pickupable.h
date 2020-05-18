// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "StructHolder.h"

class UArrowComponent;

#include "Pickupable.generated.h"

class USkeletalMeshComponent;
class UShapeComponent;

UCLASS()
class DRONEPORT_API APickupable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void AddHolder(FSTRUCT_Holder holder);
	UFUNCTION(BlueprintCallable) int ReleaseHolder(USkeletalMeshComponent* pHoldingHandMesh);
	UFUNCTION(BlueprintCallable) void UpdateGrabbedPosition();
	UFUNCTION(BlueprintCallable) void UpdatePosition();

	UFUNCTION(BlueprintCallable) void SetIdlePosComp(UArrowComponent* pIdlePosComp);
	UFUNCTION(BlueprintCallable) void SetColliderComp(UShapeComponent* pColliderComp);

protected:
	UFUNCTION() void SetSimulatePhysicsOnDrop(bool simulatePhysicsOnDrop, float timeBeforeReset = 0.f);

	UFUNCTION() virtual void OnGrab();
	UFUNCTION() virtual void OnRelease();
private:
	const int _MaxHolders = 2;
	bool _SimulatePhysicsOnDrop;
	float _TimeBeforeReset, _TimeBeforeResetMax;

	UShapeComponent* _pColliderComp;
	
protected:
	UArrowComponent* _pIdlePosComp;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FSTRUCT_Holder> _Holders;
};
