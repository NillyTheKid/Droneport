// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickupable.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/ShapeComponent.h"

// Sets default values
APickupable::APickupable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Pickupable"));
	_TimeBeforeReset = 0;
}

// Called when the game starts or when spawned
void APickupable::BeginPlay()
{
	Super::BeginPlay();
	_pColliderComp->SetSimulatePhysics(false);
}

// Called every frame
void APickupable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_TimeBeforeReset > 0)
	{
		_TimeBeforeReset -= DeltaTime;
		if (_TimeBeforeReset <= 0)
		{
			_pColliderComp->SetSimulatePhysics(false);
			SetActorLocationAndRotation(_pIdlePosComp->K2_GetComponentLocation(), _pIdlePosComp->K2_GetComponentRotation());
		}
	}
}

UFUNCTION(BlueprintCallable) void APickupable::AddHolder(FSTRUCT_Holder holder)
{
	if (_Holders.Num() == 0)
	{
		_TimeBeforeReset = 0;
		OnGrab();
	}

	if (_Holders.Add(holder) < _MaxHolders)
	{
		return;
	}

	FString errorText = "ERROR: More than 2 holders together on " + GetName();
	UKismetSystemLibrary::PrintString(GetWorld(), errorText, true, true, FLinearColor(1.f, 0.f, 0.f, 1.f), 5.0f);
}

UFUNCTION(BlueprintCallable) int APickupable::ReleaseHolder(USkeletalMeshComponent* pHoldingHandMesh)
{
	for (auto holder : _Holders)
	{
		if (holder._pHolderMesh == pHoldingHandMesh)
		{
			_Holders.Remove(holder);
			break;
		}
	}

	int nrHolders = _Holders.Num();
	if (nrHolders == 0)
	{
		OnRelease();

		if(_SimulatePhysicsOnDrop)
		{
		_TimeBeforeReset = _TimeBeforeResetMax;
		_pColliderComp->SetSimulatePhysics(true);
		}
	}

	return nrHolders;
}

UFUNCTION(BlueprintCallable) void APickupable::UpdateGrabbedPosition()
{
	auto holder = _Holders[0];
	FVector location = holder._pOffset->K2_GetComponentLocation();
	FRotator rotation = holder._pOffset->K2_GetComponentRotation();
	SetActorLocationAndRotation(location, rotation);
}

UFUNCTION(BlueprintCallable) void APickupable::UpdatePosition()
{
	if (_Holders.Num() > 0)
	{
		UpdateGrabbedPosition();
	}
	else
	{
		SetActorLocationAndRotation(_pIdlePosComp->K2_GetComponentLocation(), _pIdlePosComp->K2_GetComponentRotation());
	}
}

UFUNCTION(BlueprintCallable) void APickupable::SetIdlePosComp(UArrowComponent* pIdlePosComp)
{
	_pIdlePosComp = pIdlePosComp;
}

UFUNCTION(BlueprintCallable) void APickupable::SetColliderComp(UShapeComponent* pColliderComp)
{
	_pColliderComp = pColliderComp;
}

UFUNCTION() void APickupable::SetSimulatePhysicsOnDrop(bool simulatePhysicsOnDrop, float timeBeforeReset)
{
	_SimulatePhysicsOnDrop = simulatePhysicsOnDrop;
	_TimeBeforeResetMax = timeBeforeReset;
}

UFUNCTION() void APickupable::OnGrab()
{
}

UFUNCTION() void APickupable::OnRelease()
{
}

