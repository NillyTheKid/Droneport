// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickupable.h"

class UBoxComponent;
class UArrowComponent;
class AVolocopter;
class UPrimitiveComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class UMaterialBillboardComponent;
class UStaticMeshComponent;
class UChildActorComponent;

#include "Polaroid.generated.h"

USTRUCT()
struct FSTRUCT_PolaroidAnimation
{
	GENERATED_BODY()

public:
	FString landmarkId;
	USceneComponent* pEndSpot;
	USceneComponent* pStartSpot;
	float downDistToGo;
	float currPos;
	bool isActive;
};

UCLASS()
class DRONEPORT_API APolaroid : public APickupable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APolaroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) void TakePicture();
	UFUNCTION(BlueprintCallable) void HandleButtonCollisionEnter(UPrimitiveComponent* otherComp, bool& isFinger);
	UFUNCTION(BlueprintCallable) void HandleButtonCollisionExit(UPrimitiveComponent* otherComp);

	UFUNCTION(BlueprintCallable) void DEBUG_DrawCameraRays();
	UFUNCTION(BlueprintCallable) void SetViewPlane(UMaterialBillboardComponent* comp);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void OnSuccesfulImageTaken(FTransform& imageTransform);

	virtual void OnGrab() override;
	virtual void OnRelease() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void SetVolocopter(AVolocopter* pVolocopter);
	UFUNCTION(BlueprintCallable) void SetActorsForRayIgnore(AActor* pVolocopter, AActor* pPlayer, UChildActorComponent* pStartScreen);

	void DEBUG_EnableRenderTarget();

private:
	FVector RotateRay(float angle);
	void ClearRenderTargets();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UBoxComponent* _pBoxCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UArrowComponent* _pRayStart;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UBoxComponent* _pButtonCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) USceneCaptureComponent2D* _pSceneCaptureComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UStaticMeshComponent* _pPictureComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) float _CameraRayAngle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) float _CameraRayLength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) float _TimeBeforeResetOnDrop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) FVector _ButtonPressMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) float _ButtonPressCooldownMax;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) TMap<FString, UTextureRenderTarget2D*> _LandmarkRenderTargets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Polaroid|Animation") float _MoveDownSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Polaroid|Animation") float _AnimationSpeed;

private:
	FCollisionQueryParams _RayParams;
	AVolocopter* _pVolocopter;

	bool _IsButtonPressed;
	UBoxComponent* _pFingerCollider;
	float _ButtonPressCooldown;

	UTextureRenderTarget2D* _pViewRenderTarget;
	int _RenderTargetFramesRemaining;
	UMaterialBillboardComponent* _pViewPlaneComp;

	bool _IsActive = false;

	FSTRUCT_PolaroidAnimation _CurrAnimation;
};
