// Fill out your copyright notice in the Description page of Project Settings.


#include "Polaroid.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Landmark.h"
#include "DrawDebugHelpers.h"
#include "Volocopter.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Components/MaterialBillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"

#define RENDER_TARGET_MAX_FRAMES 2

// Sets default values
APolaroid::APolaroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	_pBoxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Collider"));
	_pRayStart = CreateDefaultSubobject<UArrowComponent>(FName("Ray Start Position"));
	_pButtonCollider = CreateDefaultSubobject<UBoxComponent>(FName("Button Collider"));
	_pSceneCaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(FName("Scene Capture 2D"));
	_pPictureComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Picture"));

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pBoxCollider->AttachToComponent(RootComponent, rules);
	_pRayStart->AttachToComponent(_pBoxCollider, rules);
	_pButtonCollider->AttachToComponent(_pBoxCollider, rules);
	_pSceneCaptureComp->AttachToComponent(_pBoxCollider, rules);
	_pPictureComp->AttachToComponent(RootComponent, rules);

	_pPictureComp->SetVisibility(false);

	SetSimulatePhysicsOnDrop(false, _TimeBeforeResetOnDrop);
}

// Called when the game starts or when spawned
void APolaroid::BeginPlay()
{
	Super::BeginPlay();
	
	_pViewRenderTarget = _pSceneCaptureComp->TextureTarget;
	ClearRenderTargets();

	_pViewPlaneComp->SetVisibility(false);
	_pSceneCaptureComp->TextureTarget = nullptr;
}

UFUNCTION(BlueprintCallable) void APolaroid::TakePicture()
{
	//Calculate ray directions
	float angle = -0.5f * _CameraRayAngle;
	FVector rayLeft = RotateRay(angle);
	angle = 0.5f * _CameraRayAngle;
	FVector rayRight = RotateRay(angle);

	//Fire rays
	//-----------------------------
	bool isPictureCorrect = false;
	auto world = GetWorld();

	//Left ray
	FHitResult result;
	FVector rayStart = _pRayStart->K2_GetComponentLocation();
	FVector rayEnd = rayStart + (rayLeft * _CameraRayLength);
	ALandmark* pLandmark = nullptr;

	//DrawDebugLine(world, rayStart, rayEnd, FColor::Red, false, 1.f);
	if (world->LineTraceSingleByChannel(result, rayStart, rayEnd, ECollisionChannel::ECC_Visibility, _RayParams))
	{
		if (result.Actor != nullptr)
		{
			if (result.Actor->ActorHasTag(FName("Landmark")))
			{
				pLandmark = (ALandmark*)result.Actor.Get();
			}
		}
	}

	//Right ray
	rayEnd = rayStart + (rayRight * _CameraRayLength);
	//DrawDebugLine(world, rayStart, rayEnd, FColor::Red, false, 1.f);
	if (world->LineTraceSingleByChannel(result, rayStart, rayEnd, ECollisionChannel::ECC_Visibility, _RayParams))
	{
		if (result.Actor != nullptr)
		{
			if (result.Actor->ActorHasTag(FName("Landmark")))
			{
				isPictureCorrect = pLandmark == (ALandmark*)result.Actor.Get();
			}
		}
	}

	if (!isPictureCorrect) return;	//TODO(Niels): Call animation for failed picture

	_pVolocopter->EnablePicture(pLandmark->_LandmarkId); //TODO(Niels): Call animation for succeeded picture
	_pSceneCaptureComp->TextureTarget = _LandmarkRenderTargets[pLandmark->_LandmarkId];
	_RenderTargetFramesRemaining = RENDER_TARGET_MAX_FRAMES;
	
	FTransform transform = _pVolocopter->GetImageLocation(pLandmark->_LandmarkId);
	OnSuccesfulImageTaken(transform);
}

UFUNCTION(BlueprintCallable) void APolaroid::HandleButtonCollisionEnter(UPrimitiveComponent* otherComp, bool& isFinger)
{
	isFinger = false;

	if (!otherComp->ComponentHasTag("Finger") || _IsButtonPressed || !_IsActive) return;

	_pButtonCollider->AddRelativeLocation(_ButtonPressMovement); //Note(Niels): Remove this when polishing
	_pFingerCollider = (UBoxComponent*)otherComp;
	_IsButtonPressed = true;

	TakePicture();

	isFinger = true;
}

UFUNCTION(BlueprintCallable) void APolaroid::HandleButtonCollisionExit(UPrimitiveComponent* otherComp)
{
	if (otherComp != _pFingerCollider) return;

	_pFingerCollider = nullptr;
	_ButtonPressCooldown = _ButtonPressCooldownMax;
}

UFUNCTION(BlueprintCallable) void APolaroid::DEBUG_DrawCameraRays()
{
	float angle = -0.5f * _CameraRayAngle;
	FVector rayLeft = RotateRay(angle);
	angle = 0.5f * _CameraRayAngle;
	FVector rayRight = RotateRay(angle);
	auto world = GetWorld();

	FVector rayStart = _pRayStart->K2_GetComponentLocation();
	FVector rayEnd = rayStart + (rayLeft * _CameraRayLength);
	DrawDebugLine(world, rayStart, rayEnd, FColor::Green);

	rayEnd = rayStart + (rayRight * _CameraRayLength);
	DrawDebugLine(world, rayStart, rayEnd, FColor::Green);
}

UFUNCTION(BlueprintCallable) void APolaroid::SetViewPlane(UMaterialBillboardComponent* comp)
{
	_pViewPlaneComp = comp;
}

UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void APolaroid::OnSuccesfulImageTaken_Implementation(FTransform& imageTransform)
{
	imageTransform = FTransform();
}


void APolaroid::OnGrab()
{
	_pViewPlaneComp->SetVisibility(true);
	_pSceneCaptureComp->TextureTarget = _pViewRenderTarget;
	_IsActive = true;
}

void APolaroid::OnRelease()
{
	_pViewPlaneComp->SetVisibility(false);
	_pSceneCaptureComp->TextureTarget = nullptr;
	_IsActive = false;
}

// Called every frame
void APolaroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_ButtonPressCooldown > 0)
	{
		_ButtonPressCooldown -= DeltaTime;
		if (_IsButtonPressed && _ButtonPressCooldown <= 0)
		{
			_IsButtonPressed = false;
			_pButtonCollider->AddRelativeLocation(-_ButtonPressMovement);
		}
	}

	if (_RenderTargetFramesRemaining > 0)
	{
		_RenderTargetFramesRemaining--;
		if (_RenderTargetFramesRemaining == 0)
		{
			_pSceneCaptureComp->TextureTarget = _pViewRenderTarget;
		}
	}
}

UFUNCTION(BlueprintCallable) void APolaroid::SetVolocopter(AVolocopter* pVolocopter)
{
	_pVolocopter = pVolocopter;
}

UFUNCTION(BlueprintCallable) void APolaroid::SetActorsForRayIgnore(AActor* pVolocopter, AActor* pPlayer, UChildActorComponent* pStartScreen)
{
	_RayParams.AddIgnoredActor(pVolocopter);
	_RayParams.AddIgnoredActor(pPlayer);
	_RayParams.AddIgnoredActor(pStartScreen->GetChildActor());
	_RayParams.AddIgnoredActor(this);
}

void APolaroid::DEBUG_EnableRenderTarget()
{
	_pSceneCaptureComp->TextureTarget = _pViewRenderTarget;
}

FVector APolaroid::RotateRay(float angle)
{
	FVector result = _pRayStart->GetForwardVector();
	float radAngle = FMath::DegreesToRadians(angle);

	//Rotate around Y axis
	result = FVector(
		result.X * FMath::Cos(radAngle) + result.Z * FMath::Sin(radAngle),
		result.Y, 
		-result.X * FMath::Sin(radAngle) + result.Z * FMath::Cos(radAngle));

	//Rotate around Z axis
	result = FVector(
		result.X * FMath::Cos(radAngle) - result.Y * FMath::Sin(radAngle),
		result.X * FMath::Sin(radAngle) + result.Y * FMath::Cos(radAngle),
		result.Z);

	return result;
}

void APolaroid::ClearRenderTargets()
{
	auto world = GetWorld();

	for (auto rt : _LandmarkRenderTargets)
	{
		UKismetRenderingLibrary::ClearRenderTarget2D(world, rt.Value);
	}
}

