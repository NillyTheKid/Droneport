// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Volocopter.generated.h"

class AVolocopterPath;
class APlayerBase;
class AFlightStickTarget;
class APdaColourApp;
class APolaroid;
class APdaBase;

class UArrowComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UMaterialParameterCollectionInstance;
class USplineComponent;

UENUM(BlueprintType)
enum class ENUM_UserFlightInput : uint8 
{
	INPUT_HORIZONTAL,
	INPUT_VERTICAL
};

USTRUCT()
struct FSTRUCT_AutoPilot
{
	GENERATED_BODY()

public:
	float ascentTime;
	float hoverTime;
	float hoverSpeed;
	float descentTime;
	bool isActive;
};

UCLASS()
class DRONEPORT_API AVolocopter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVolocopter();

	UFUNCTION(BlueprintCallable) void SetPlayer(APlayerBase* pPlayer);
	UFUNCTION(BlueprintCallable) void SetFlightStickTarget(AFlightStickTarget* pFlightStickTarget);
	UFUNCTION(BlueprintCallable) void GetFlightStickTarget(AFlightStickTarget* &pFlightStickTarget);

	UFUNCTION(BlueprintCallable)
		void GetArUiParams(float& etaMinutes, float& etaSeconds, float& spottedLandmarks, float& maxLandmarks, float& temperature,
			float& speed, float& speedMax, float& speedConstant, float& altitude, FLinearColor& colour);
	
	UMETA(DeprecatedNode) UFUNCTION() void Reset();
	UFUNCTION(BlueprintCallable) void DEBUG_SetPolaroid(APolaroid* pPolaroid);

	UFUNCTION(BlueprintCallable) void SetIsVolocopterMoving(bool isVolocopterMoving);
	UFUNCTION(BlueprintCallable) APdaBase* GetPda();

	FTransform GetImageLocation(FString id);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

	UFUNCTION(BlueprintCallable) void Move(float elapsedSec);
	UFUNCTION(BlueprintCallable) void UpdatePlayerPos();
	UFUNCTION(BlueprintCallable) void UpdateFlightStickRotation();
	UMETA(DeprecatedNode) UFUNCTION(BlueprintCallable) void UpdateWindshieldUi();

	UFUNCTION(BlueprintCallable) void ToggleUserFlight();
	UFUNCTION(BlueprintCallable) void SetUserFlightInput(ENUM_UserFlightInput inputAxis, float value);
	UFUNCTION(BlueprintCallable) void ClearUserFlightInput();
	UFUNCTION(BlueprintCallable) void UpdateUserFlightInput();
	UFUNCTION(BlueprintCallable) void UpdateVolocopterRotation();

	UFUNCTION(BlueprintCallable) void RegisterPicture(UStaticMeshComponent* picture, FString id);
	UFUNCTION(BlueprintCallable) void SetPda(UChildActorComponent* pPdaComp);

	UFUNCTION(BlueprintCallable) void SetColourApp(UChildActorComponent* pPdaComp);

	UFUNCTION(BlueprintCallable) void DEBUG_Look(ENUM_UserFlightInput inputAxis, float value);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetPath(AVolocopterPath* pPath);
	FRotator GetPlayerWorldRotation() const;

	void EnablePicture(FString id);

	void GetPictureData(FString id, USceneComponent*& pPictureComp, UMaterialInterface*& pMat);

private:
	void HandleUserFlight(float elapsedSec);
	void HandleAcceleration(float& velocity, float acceleration, float targetSpeed);
	void CalculateUserpilot(float targetDist);

	float GetSignedDistance(const FVector& point, const FVector& centerPoint, const FVector& posPoint, const FVector& negPoint);
	void CalculateEta();
	void DEBUG_LookUpdate(float elapsedSec);
	void UpdateFlightStickLocalPos();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float _Speed = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _MaxDistFromSpline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _UserFlightAcceleration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _UserFlightMaxSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _MaxTilt;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _LeftRightSensitivtyMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _VerticalMovementTiltMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _RotationRollMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _PreviousPosDistance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _MaxDeltaRot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|User Flight") float _MaxDeltaPosZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|AR Windshield") UMaterialParameterCollection* _pMatParamCollection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|AR Windshield") float _Temperature;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|AR Windshield") float _BaseSpeedDisplay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Volocopter|AR Windshield") float _MaxUserSpeedDisplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) float _LookRotationSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) FVector _DEBUG_PolaroidOffset = FVector(30, 0, -5);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UStaticMeshComponent* _pMeshComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UArrowComponent* _pPlayerPosComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UArrowComponent* _pPolaroidIdlePosComp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UArrowComponent* _pFlightStickGrabPos;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) USceneComponent* _pFlightStickLocalPos;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) bool _IsVolocopterMoving = true;

private:
	AVolocopterPath* _pPath;
	APlayerBase* _pPlayer;
	FVector _UserFlightOffset, _UserFlightVelocity;
	FVector _UserFlightInput;
	FSTRUCT_AutoPilot _UserFlightAutopilot;
	TMap<FString, UStaticMeshComponent*> _pPictures; //TODO(Niels): Replace placeholders with location components
	AFlightStickTarget* _pFlightStickTarget;
	UMaterialParameterCollectionInstance* _pMatParamCollectionInst;
	APdaColourApp* _pPdaColourApp;
	APdaBase* _pPda;

	FVector _LookAroundInput;
	bool _IsUsingDebugControls;

	float _EstimatedTimeArrival;
	int _MaxLandmarks = 0, _CollectedLandmarks = 0;
	TArray<FString> _TakenPictures;

	USplineComponent* _pSpline;
	FVector _PrevPosOnSpline, _CurrPosOnSpline;
	FRotator _PrevRotOnSpline, _CurrRotOnSpline;
	FVector _PrevDeltaPos;
	float _PrevDeltaRot;

	APolaroid* _p_DEBUG_Polaroid;
};
