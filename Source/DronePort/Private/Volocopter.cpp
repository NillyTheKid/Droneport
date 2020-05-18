// Fill out your copyright notice in the Description page of Project Settings.


#include "Volocopter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "VolocopterPath.h"
#include "Components/SplineComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerBase.h"
#include "FlightStickTarget.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "PdaBase.h"
#include "PdaColourApp.h"
#include "Polaroid.h"

// Sets default values
AVolocopter::AVolocopter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_pMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Volocopter"));
	_pPlayerPosComp = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerPos"));
	_pPolaroidIdlePosComp = CreateDefaultSubobject<UArrowComponent>(TEXT("PolaroidIdlePos"));
	_pFlightStickGrabPos = CreateDefaultSubobject<UArrowComponent>(TEXT("Flight Stick Grabber Pos"));
	_pFlightStickLocalPos = CreateDefaultSubobject<USceneComponent>(TEXT("Flight Stick Local Pos"));

	//Attach components
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pMeshComp->AttachToComponent(RootComponent, rules);
	_pPlayerPosComp->AttachToComponent(_pMeshComp, rules);
	_pPolaroidIdlePosComp->AttachToComponent(_pMeshComp, rules);
	_pFlightStickGrabPos->AttachToComponent(_pMeshComp, rules);
	_pFlightStickLocalPos->AttachToComponent(_pMeshComp, rules);
}

UFUNCTION(BlueprintCallable) void AVolocopter::SetPlayer(APlayerBase* pPlayer)
{
	_pPlayer = pPlayer;
	_pPlayer->SetVolocopter(this);
}

UFUNCTION(BlueprintCallable) void AVolocopter::SetFlightStickTarget(AFlightStickTarget* pFlightStickTarget)
{
	_pFlightStickTarget = pFlightStickTarget;
	pFlightStickTarget->SetIdlePosComp(_pFlightStickGrabPos);
}

UFUNCTION(BlueprintCallable) void AVolocopter::GetFlightStickTarget(AFlightStickTarget*& pFlightStickTarget)
{
	pFlightStickTarget = pFlightStickTarget;
}

void AVolocopter::GetArUiParams(float& etaMinutes, float& etaSeconds, float& spottedLandmarks, float& maxLandmarks, float& temperature, float& speed, float& speedMax, float& speedConstant, float& altitude, FLinearColor& colour)
{
	//Update ETA
	etaMinutes = int(_EstimatedTimeArrival) / 60;
	etaSeconds = int(_EstimatedTimeArrival) % 60;

	//Update Nr Landmarks
	spottedLandmarks = _CollectedLandmarks;
	maxLandmarks = _MaxLandmarks;

	//Update Temperature
	temperature = _Temperature;

	//Update Speed
	if (_IsVolocopterMoving)
	{
		speed = _BaseSpeedDisplay + ((_UserFlightVelocity.Size() / _UserFlightMaxSpeed) * _MaxUserSpeedDisplay);
		speedMax = _BaseSpeedDisplay + _MaxUserSpeedDisplay;
		speedConstant = _BaseSpeedDisplay;
	}
	else
	{
		speed = 0;
		speedMax = 100;
		speedConstant = 0;
	}

	//Update Altitude
	altitude = GetActorLocation().Z;

	//Update Colour
	if (_pPdaColourApp != nullptr)
	{
		_pPdaColourApp->GetColour(colour);
	}
}

UFUNCTION() void AVolocopter::Reset() //Deprecated 
{
	_PrevDeltaPos = FVector();
	_PrevDeltaRot = 0;
	_CurrPosOnSpline = _PrevPosOnSpline = FVector();
	_CurrRotOnSpline = _PrevRotOnSpline = FRotator();

	SetActorRotation(FRotator());
	_pMeshComp->SetRelativeRotation(FRotator());

	_CollectedLandmarks = 0;
	_TakenPictures.Empty();

	for (auto p : _pPictures)
	{
		p.Value->SetVisibility(false);
	}

	_UserFlightOffset = FVector(0, 0, 0);
	_UserFlightVelocity = FVector(0, 0, 0);
	_UserFlightInput = FVector();
	_UserFlightAutopilot = FSTRUCT_AutoPilot();

	if (_pPdaColourApp != nullptr)
	{
		_pPdaColourApp->Reset();
	}
	if (_pPlayer != nullptr)
	{
		_pPlayer->Reset();
	}
}

UFUNCTION(BlueprintCallable) void AVolocopter::DEBUG_SetPolaroid(APolaroid* pPolaroid)
{
	_p_DEBUG_Polaroid = pPolaroid;
}

UFUNCTION(BlueprintCallable) void AVolocopter::SetIsVolocopterMoving(bool isVolocopterMoving)
{
	_IsVolocopterMoving = isVolocopterMoving;
}

UFUNCTION(BlueprintCallable)APdaBase* AVolocopter::GetPda()
{
	return _pPda;
}

FTransform AVolocopter::GetImageLocation(FString id)
{
	return _pPictures[id]->GetComponentTransform();
}

// Called when the game starts or when spawned
void AVolocopter::BeginPlay()
{
	Super::BeginPlay();
	
	_UserFlightInput = FVector(0, 0, 0);

	_UserFlightAutopilot.isActive = false;

	//_pPlayerPosComp->AddLocalRotation(FRotator(_MaxTilt, 0, 0));
}

void AVolocopter::OnConstruction(const FTransform& transform)
{
	auto world = GetWorld();
	
	//Disabled until this gets implemented into the game
	//_pMatParamCollectionInst = world->GetParameterCollectionInstance(_pMatParamCollection);
}

UFUNCTION(BlueprintCallable) void AVolocopter::Move(float elapsedSec)
{
	if (!_IsVolocopterMoving || _pPath == nullptr) return;

	CalculateEta();

	float distToMove = elapsedSec * _Speed;
	float distAlongSpline = _pPath->UpdatePosition(distToMove);

	float prevPos = distAlongSpline - _PreviousPosDistance;
	if (prevPos < 0)
	{
		prevPos = 0;
	}

	_PrevPosOnSpline = _pSpline->GetLocationAtDistanceAlongSpline(prevPos, ESplineCoordinateSpace::World);
	_PrevRotOnSpline = _pSpline->GetRotationAtDistanceAlongSpline(prevPos, ESplineCoordinateSpace::World);
	_CurrPosOnSpline = _pSpline->GetLocationAtDistanceAlongSpline(distAlongSpline, ESplineCoordinateSpace::World);
	_CurrRotOnSpline = _pSpline->GetRotationAtDistanceAlongSpline(distAlongSpline, ESplineCoordinateSpace::World);

	HandleUserFlight(elapsedSec);

	FVector pos = _CurrPosOnSpline;
	FRotator rot = FRotator(0, _CurrRotOnSpline.Yaw, 0);
	SetActorLocationAndRotation(pos, rot);

	AddActorLocalOffset(_UserFlightOffset);
}

UFUNCTION(BlueprintCallable) void AVolocopter::UpdatePlayerPos()
{
	if (_pPlayer != nullptr)
	{
		((AActor*)_pPlayer)->SetActorTransform(_pPlayerPosComp->GetComponentTransform());
	}
}

UFUNCTION(BlueprintCallable) void AVolocopter::UpdateFlightStickRotation()
{
	if (_pFlightStickTarget == nullptr) return;

	_pFlightStickTarget->UpdatePosition();
	_pFlightStickTarget->ClampPosition();

	//FHitResult result;
	//auto forward = _pFlightStickTarget->GetActorLocation() - _pFlightStickComp->K2_GetComponentLocation();
	//auto up = GetActorForwardVector();
	//auto rot = UKismetMathLibrary::MakeRotFromXZ(forward.GetSafeNormal(), up.GetSafeNormal());

	//_pFlightStickComp->K2_SetWorldRotation(rot, false, result, false);
}

UFUNCTION(BlueprintCallable) void AVolocopter::UpdateWindshieldUi()
{
}

UFUNCTION(BlueprintCallable) void AVolocopter::ToggleUserFlight()
{
	//_IsUserFlightEnabled = !_IsUserFlightEnabled;
}

UFUNCTION(BlueprintCallable) void AVolocopter::SetUserFlightInput(ENUM_UserFlightInput inputAxis, float value)
{
	switch (inputAxis)
	{
	case ENUM_UserFlightInput::INPUT_HORIZONTAL:
		_UserFlightInput.Y = value;
		break;
	case ENUM_UserFlightInput::INPUT_VERTICAL:
		_UserFlightInput.Z = value;
		break;
	}
}

UFUNCTION(BlueprintCallable) void AVolocopter::ClearUserFlightInput()
{
	_UserFlightInput = FVector(0, 0, 0);
}

UFUNCTION(BlueprintCallable) void AVolocopter::UpdateUserFlightInput()
{
	FVector grabPos = _pFlightStickGrabPos->RelativeLocation;

	if (_pFlightStickTarget != nullptr)
	{
		FVector targetPos = _pFlightStickLocalPos->RelativeLocation;

		float forwardDistance = targetPos.X - grabPos.X;
		float rightDistance = -(targetPos.Y - grabPos.Y);

		FVector2D input = FVector2D(forwardDistance, rightDistance);
		FVector2D scaledInput = _pFlightStickTarget->ScaleInput(input);

		_UserFlightInput = FVector(0, scaledInput.Y * _LeftRightSensitivtyMultiplier, scaledInput.X);
	}
}

UFUNCTION(BlueprintCallable) void AVolocopter::UpdateVolocopterRotation()
{
	//float deltaRot = _CurrRotOnSpline.Yaw - _PrevRotOnSpline.Yaw;
	//FVector deltaPos = _CurrPosOnSpline - _PrevPosOnSpline;
	//FRotator tilt(0, 0, 0);
	//
	//if (FMath::Abs(deltaRot - _PrevDeltaRot) > _MaxDeltaRot)
	//{
	//	if (deltaRot - _PrevDeltaRot > 0)
	//	{
	//		deltaRot = _PrevDeltaRot + _MaxDeltaRot;
	//	}
	//	else
	//	{
	//		deltaRot = _PrevDeltaRot - _MaxDeltaRot;
	//	}
	//}
	//FVector deltaDeltaPos = deltaPos - _PrevDeltaPos;

	//if (FMath::Abs(deltaDeltaPos.Z) > _MaxDeltaPosZ)
	//{
	//	if (deltaDeltaPos.Z > 0)
	//	{
	//		deltaPos.Z = _PrevDeltaPos.Z + _MaxDeltaPosZ;
	//	}
	//	else
	//	{
	//		deltaPos.Z = _PrevDeltaPos.Z - _MaxDeltaPosZ;
	//	}
	//}

	////Forward Tilt
	//tilt.Pitch = _MaxTilt;

	////Vertical Movement Tilt
	//tilt.Pitch -= deltaPos.Z * _VerticalMovementTiltMultiplier * _MaxTilt;

	////Rotation Roll
	//tilt.Roll = deltaRot * _RotationRollMultiplier * -_MaxTilt;

	////Sideways Movement Roll
	//tilt.Roll;

	////Rotate Volocopter to face correct direction
	//tilt.Yaw = 180;

	////Apply rotation
	//FHitResult hitResult;
	//_pMeshComp->K2_SetRelativeRotation(tilt, false, hitResult, false);

	////UKismetSystemLibrary::PrintString(GetWorld(), tilt.ToString());

	//_PrevDeltaPos = deltaPos;
	//_PrevDeltaRot = deltaRot;

	if (_UserFlightAutopilot.isActive) return;

	FRotator rot = FRotator(0, 180, 0);
	rot.Roll = -(_UserFlightVelocity.Y / _UserFlightMaxSpeed) * _MaxTilt;
	rot.Pitch = -(_UserFlightVelocity.Z / _UserFlightMaxSpeed) * _MaxTilt;

	_pMeshComp->SetRelativeRotation(rot);
}

UFUNCTION(BlueprintCallable) void AVolocopter::RegisterPicture(UStaticMeshComponent* picture, FString id)
{
	++_MaxLandmarks;
	_pPictures.Add(id, picture);
}

UFUNCTION(BlueprintCallable) void AVolocopter::SetPda(UChildActorComponent* pPdaComp)
{
	_pPda = (APdaBase*)pPdaComp->GetChildActor();
}

UFUNCTION(BlueprintCallable) void AVolocopter::SetColourApp(UChildActorComponent* pPdaComp)
{
	auto pda = ((APdaBase*)pPdaComp->GetChildActor());
	_pPdaColourApp = pda->GetColourApp();
}

UFUNCTION(BlueprintCallable) void AVolocopter::DEBUG_Look(ENUM_UserFlightInput inputAxis, float value)
{
	switch (inputAxis)
	{
	case ENUM_UserFlightInput::INPUT_HORIZONTAL:
	{
		_LookAroundInput.X = value;
	} break;
	case ENUM_UserFlightInput::INPUT_VERTICAL:
	{
		_LookAroundInput.Y = value;
	} break;
	}

	if (!_IsUsingDebugControls && value != 0)
	{
		if (_pPlayer != nullptr)
		{
			_IsUsingDebugControls = true;
			_pPlayer->DEBUG_DisableHands();

			FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
			_pPolaroidIdlePosComp->AttachToComponent(_pPlayerPosComp, rules);
			_pPolaroidIdlePosComp->SetRelativeLocation(_DEBUG_PolaroidOffset);
			_pPolaroidIdlePosComp->SetRelativeRotation(FRotator(0, 0, 0));
			
			_p_DEBUG_Polaroid->DEBUG_EnableRenderTarget();
		}
	}
}

// Called every frame
void AVolocopter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFlightStickLocalPos();

	if (_IsUsingDebugControls)
	{
		DEBUG_LookUpdate(DeltaTime);
	}
}

void AVolocopter::SetPath(AVolocopterPath* pPath)
{
	_pPath = pPath;
	_pSpline = _pPath->GetSpline();

	CalculateEta();

	_PrevPosOnSpline = _CurrPosOnSpline = _pSpline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
	_PrevRotOnSpline = _CurrRotOnSpline = _pSpline->GetRotationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
}

FRotator AVolocopter::GetPlayerWorldRotation() const
{
	return _pPlayerPosComp->K2_GetComponentRotation();
}

void AVolocopter::EnablePicture(FString id)
{
	if (_TakenPictures.Find(id) == INDEX_NONE)
	{
		++_CollectedLandmarks;
		_pPictures[id]->SetVisibility(true);
		_TakenPictures.Add(id);
	}
}

void AVolocopter::GetPictureData(FString id, USceneComponent*& pPictureComp, UMaterialInterface*& pMat)
{
}

void AVolocopter::HandleUserFlight(float elapsedSec)
{
	//Get targetspeed for acceleration
	FVector targetSpeed = FVector(0,0,0);
	if (_UserFlightOffset.Size() <= _MaxDistFromSpline)
	{
		_UserFlightAutopilot.isActive = false;

		if (_UserFlightInput.Size() > 1)
		{
			targetSpeed = _UserFlightInput.GetSafeNormal() * _UserFlightMaxSpeed;
		}
		else
		{
			targetSpeed = _UserFlightInput * _UserFlightMaxSpeed;
		}
	}
	else
	{
		if (_UserFlightAutopilot.isActive)
		{
			if (_UserFlightAutopilot.ascentTime > 0)
			{
				targetSpeed = -_UserFlightOffset.GetSafeNormal() * _UserFlightAutopilot.hoverSpeed;
				_UserFlightAutopilot.ascentTime -= elapsedSec;
			}
			else if (_UserFlightAutopilot.hoverTime > 0)
			{
				targetSpeed = -_UserFlightOffset.GetSafeNormal() * _UserFlightAutopilot.hoverSpeed;
				_UserFlightAutopilot.hoverTime -= elapsedSec;
			}
			else if (_UserFlightAutopilot.descentTime > 0)
			{
				targetSpeed = FVector(0, 0, 0);
				_UserFlightAutopilot.descentTime -= elapsedSec;
			}
			else
			{
				_UserFlightAutopilot.isActive = false;
			}
		}
		else if (_UserFlightVelocity.Size() == 0)
		{
			CalculateUserpilot(_UserFlightOffset.Size() - _MaxDistFromSpline);
		}
	}

	//Handle acceleration/deceleration
	auto accel = _UserFlightAcceleration * elapsedSec;
	HandleAcceleration(_UserFlightVelocity.Y, accel, targetSpeed.Y);
	HandleAcceleration(_UserFlightVelocity.Z, accel, targetSpeed.Z);

	//Handle Velocity
	_UserFlightOffset += _UserFlightVelocity * elapsedSec;

	//Handle Location
	//_UserFlightOffset = (_UserFlightOffset.Y * GetActorRightVector()) + (_UserFlightOffset.Z * GetActorUpVector());
}

void AVolocopter::HandleAcceleration(float& velocity, float acceleration, float targetSpeed)
{
	float deltaSpeed = targetSpeed - velocity;
	float absDeltaSpeed = FGenericPlatformMath::Abs(deltaSpeed);

	if (acceleration < absDeltaSpeed)
	{
		if (deltaSpeed < 0)
		{
			velocity -= acceleration;
		}
		else
		{
			velocity += acceleration;
		}
	}
	else
	{
		velocity = targetSpeed;
	}
}

//Will set a course for a point in the opposite direction of the pathOffset vector along length targetDist 
void AVolocopter::CalculateUserpilot(float targetDist)
{
	float hoverSpeed = FMath::Sqrt(targetDist*_UserFlightAcceleration);
	if (hoverSpeed > _UserFlightMaxSpeed)
	{
		hoverSpeed = _UserFlightMaxSpeed;
		float accelDist = FMath::Pow(hoverSpeed, 2) / _UserFlightAcceleration;
		_UserFlightAutopilot.ascentTime = FMath::Sqrt(accelDist / _UserFlightAcceleration);
		_UserFlightAutopilot.descentTime = _UserFlightAutopilot.ascentTime;
		
		float hoverDist = targetDist - (accelDist * 2);
		_UserFlightAutopilot.hoverTime = hoverDist / hoverSpeed;
	}
	else
	{
		_UserFlightAutopilot.ascentTime = FMath::Sqrt(targetDist/ _UserFlightAcceleration);
		_UserFlightAutopilot.descentTime = _UserFlightAutopilot.ascentTime;
		_UserFlightAutopilot.hoverTime = 0.0f;
	}

	_UserFlightAutopilot.hoverSpeed = hoverSpeed;
	_UserFlightAutopilot.isActive = true;
}

float AVolocopter::GetSignedDistance(const FVector& point, const FVector& centerPoint, const FVector& posPoint, const FVector& negPoint)
{
	float dist = FVector::Distance(point, centerPoint);
	float posDist = FVector::Distance(point, posPoint);
	float negDist = FVector::Distance(point, negPoint);

	if (negDist < posDist)
	{
		dist *= -1;
	}

	return dist;
}

void AVolocopter::CalculateEta()
{
	if (_pSpline == nullptr || _pPath == nullptr) return;

	auto dist = _pSpline->GetSplineLength();
	auto currPos = _pPath->UpdatePosition(0);
	_EstimatedTimeArrival = (dist - currPos) / _Speed;
}

void AVolocopter::DEBUG_LookUpdate(float elapsedSec)
{
	_LookAroundInput *= _LookRotationSpeed * elapsedSec;
	_pPlayerPosComp->AddRelativeRotation(FRotator(_LookAroundInput.Y, _LookAroundInput.X, 0));

	_LookAroundInput = FVector();
}

void AVolocopter::UpdateFlightStickLocalPos()
{
	if (_pFlightStickTarget != nullptr)
	{
		FHitResult result;
		_pFlightStickLocalPos->K2_SetWorldLocation(_pFlightStickTarget->K2_GetActorLocation(), false, result, false);
	}
}

