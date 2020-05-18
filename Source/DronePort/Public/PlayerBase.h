// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "EnumPlayerHandPart.h"
#include "EnumHandState.h"
#include "EnumPlayerHandsState.h"

class UBoxComponent;
class USkeletalMeshComponent;
class APickupable;
class AVolocopter;
class UArrowComponent;

class UMotionControllerComponent;
class UCameraComponent;
class APdaBase;

#include "PlayerBase.generated.h"

UCLASS()
class DRONEPORT_API APlayerBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

	void SetVolocopter(AVolocopter* pVolocopter);
	void DEBUG_DisableHands();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* pPlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent) void HandleGameReset();
	void SetPda(APdaBase* pPda);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) void SetAnimState(ENUM_PlayerHandPart part, ENUM_HandState newState, USkeletalMeshComponent* pHand);
	UFUNCTION(BlueprintCallable) void RecalibrateHmd();

	UFUNCTION(BlueprintCallable) void SetHandState(ENUM_PlayerHandsState state);
	UFUNCTION(BlueprintCallable) void ClearHandState(ENUM_PlayerHandsState state);
	UFUNCTION(BlueprintCallable)
		void CheckForGrabCollision(UBoxComponent* pCollider, ENUM_PlayerHandsState coreState);
	UFUNCTION(BlueprintCallable)
		void TryGrab(AActor* pOtherActor, USkeletalMeshComponent* pHandMesh, UBoxComponent* pCollider, USceneComponent* pGrabOffset);
	UFUNCTION(BlueprintCallable) void TryRelease(USkeletalMeshComponent* pHandMesh, UBoxComponent* pCollider);
	UFUNCTION(BlueprintCallable) void CheckForFingerCollision(bool doCheckRightHand);

private:
	UFUNCTION() void CheckHandsState();
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "Controller_L") UMotionControllerComponent* _pControllerLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "Controller_R") UMotionControllerComponent* _pControllerRight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "Camera") UCameraComponent* _pCamera;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "LeftHand") USkeletalMeshComponent* _pLeftHandMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "LeftHandGrabbedOffset") USceneComponent* _pLeftGrabOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "RightHand") USkeletalMeshComponent* _pRightHandMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "RightHandGrabbedOffset") USceneComponent* _pRightGrabOffset;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "LeftHandIndexPivot") USceneComponent* _pLeftIndexPivot;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "GrabCollider_L") UBoxComponent* _pGrabColliderLeft;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "RightHandIndexPivot") USceneComponent* _pRightIndexPivot;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "GrabCollider_R") UBoxComponent* _pGrabColliderRight;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "LeftHandIndex") UBoxComponent* _pLeftIndexCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, DisplayName = "RightHandIndex") UBoxComponent* _pRightIndexCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) uint8 _HandsState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) APickupable* _pGrabbedObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) APdaBase* _pPda;

private:
	AVolocopter* _pVolocopter;

	bool _IsLeftFingerActive = false, _IsRightFingerActive = false;
	int _LeftColliderActive = 0, _RightColliderActive = 0;
};
