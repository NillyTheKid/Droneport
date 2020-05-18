// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "VrHandsAnimInstance.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Pickupable.h"
#include "Volocopter.h"
#include "Components/ArrowComponent.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "PdaBase.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create components
	RootComponent = CreateDefaultSubobject<USceneComponent>("Default Root");
	_pControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>("ControllerLeft");
	_pControllerRight = CreateDefaultSubobject<UMotionControllerComponent>("ControllerRight");
	_pCamera = CreateDefaultSubobject<UCameraComponent>("Camera");
	_pLeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LeftHandMesh");
	_pLeftGrabOffset = CreateDefaultSubobject<USceneComponent>("LeftGrabOffset");
	_pRightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RightHandMesh");
	_pRightGrabOffset = CreateDefaultSubobject<USceneComponent>("RightGrabOffset");
	_pLeftIndexPivot = CreateDefaultSubobject<USceneComponent>("LeftIndexPivot");
	_pGrabColliderLeft = CreateDefaultSubobject<UBoxComponent>("LeftGrabCollider");
	_pRightIndexPivot = CreateDefaultSubobject<USceneComponent>("RightIndexPivot");
	_pGrabColliderRight = CreateDefaultSubobject<UBoxComponent>("RightGrabCollider");
	_pLeftIndexCollider = CreateDefaultSubobject<UBoxComponent>("LeftIndexCollider");
	_pRightIndexCollider = CreateDefaultSubobject<UBoxComponent>("RightIndexCollider");

	//Attach components
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pControllerLeft->AttachToComponent(RootComponent, rules);
	_pControllerRight->AttachToComponent(RootComponent,rules);
	_pCamera->AttachToComponent(RootComponent, rules);
	_pLeftHandMesh->AttachToComponent(_pControllerLeft, rules);
	_pLeftGrabOffset->AttachToComponent(_pControllerLeft, rules);
	_pRightHandMesh->AttachToComponent(_pControllerRight, rules);
	_pRightGrabOffset->AttachToComponent(_pControllerRight, rules);
	_pLeftIndexPivot->AttachToComponent(_pLeftHandMesh, rules);
	_pGrabColliderLeft->AttachToComponent(_pLeftHandMesh, rules);
	_pRightIndexPivot->AttachToComponent(_pRightHandMesh, rules);
	_pGrabColliderRight->AttachToComponent(_pRightHandMesh, rules);
	_pLeftIndexCollider->AttachToComponent(_pLeftIndexPivot, rules);
	_pRightIndexCollider->AttachToComponent(_pRightIndexPivot, rules);
	
	//Set Tags
	_pLeftIndexCollider->ComponentTags.Add("Finger");
	_pLeftIndexCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_pRightIndexCollider->ComponentTags.Add("Finger");
	_pRightIndexCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerBase::SetVolocopter(AVolocopter* pVolocopter)
{
	_pVolocopter = pVolocopter;
}

void APlayerBase::DEBUG_DisableHands()
{
	_pLeftHandMesh->SetVisibility(false);
	_pRightHandMesh->SetVisibility(false);
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckHandsState();
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* pPlayerInputComponent)
{
	Super::SetupPlayerInputComponent(pPlayerInputComponent);

}

void APlayerBase::SetPda(APdaBase* pPda)
{
	_pPda = pPda;
}

UFUNCTION(BlueprintNativeEvent) void APlayerBase::HandleGameReset_Implementation()
{
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UFUNCTION(BlueprintCallable) void APlayerBase::SetAnimState(ENUM_PlayerHandPart part, ENUM_HandState newState, USkeletalMeshComponent* pHand)
{
	auto animInstance = (UVrHandsAnimInstance*)(pHand->GetAnimInstance());
	switch (part)
	{
	case ENUM_PlayerHandPart::CORE:
		animInstance->_HandState = newState;
		break;
	case ENUM_PlayerHandPart::INDEX:
		animInstance->_IndexState = newState;
		break;
	}
}

UFUNCTION(BlueprintCallable) void APlayerBase::RecalibrateHmd()
{
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

UFUNCTION(BlueprintCallable) void APlayerBase::SetHandState(ENUM_PlayerHandsState state)
{
	_HandsState |= (1 << int(state));
}

UFUNCTION(BlueprintCallable) void APlayerBase::ClearHandState(ENUM_PlayerHandsState state)
{
	_HandsState &= ~(1 << int(state));
}

void APlayerBase::CheckForGrabCollision(UBoxComponent* pCollider, ENUM_PlayerHandsState coreState)
{
	uint8 flag = 1 << int(coreState);
	if ((_HandsState & flag) == flag)
	{
		pCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	if (coreState == ENUM_PlayerHandsState::LEFT_CORE)
	{
		_LeftColliderActive = 3;
	}
	else
	{
		_RightColliderActive = 3;
	}
}

UFUNCTION(BlueprintCallable)
void APlayerBase::TryGrab(AActor* pOtherActor, USkeletalMeshComponent* pHandMesh, UBoxComponent* pCollider, USceneComponent* pGrabOffset)
{
	if (!pOtherActor->ActorHasTag("Pickupable")) return;

	FSTRUCT_Holder holder{};
	holder._pHolderMesh = pHandMesh;
	FHitResult result;
	pGrabOffset->K2_SetWorldLocation(pOtherActor->GetActorLocation(), false, result , false);
	pGrabOffset->K2_SetWorldRotation(pOtherActor->GetActorRotation(), false, result, false);
	holder._pOffset = pGrabOffset;

	if (_pGrabbedObject == nullptr)
	{
		auto pPickupable = ((APickupable*)pOtherActor);
		_pGrabbedObject = pPickupable;
	}

	_pGrabbedObject->AddHolder(holder);
	pCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UFUNCTION(BlueprintCallable) void APlayerBase::TryRelease(USkeletalMeshComponent* pHandMesh, UBoxComponent* pCollider)
{
	pCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (_pGrabbedObject == nullptr) return;

	if (_pGrabbedObject->ReleaseHolder(pHandMesh) == 0)
	{
		_pGrabbedObject = nullptr;
	}
}

UFUNCTION(BlueprintCallable) void APlayerBase::CheckForFingerCollision(bool doCheckRightHand)
{
	if (doCheckRightHand)
	{
		uint8 flag = 1 << int(ENUM_PlayerHandsState::RIGHT_INDEX);
		if ((_HandsState & flag) != flag)
		{
			_pRightIndexCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			return;
		}

		_pRightIndexCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		uint8 flag = 1 << int(ENUM_PlayerHandsState::LEFT_INDEX);
		if ((_HandsState & flag) != flag)
		{
			_pLeftIndexCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			return;
		}
		_pLeftIndexCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

UFUNCTION() void APlayerBase::CheckHandsState()
{
	if (_LeftColliderActive > 0)
	{
		if (--_LeftColliderActive == 0)
		{
			_pGrabColliderLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	if (_RightColliderActive > 0)
	{
		if (--_RightColliderActive == 0)
		{
			_pGrabColliderRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}
