// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class UStaticMesh;
class UBoxComponent;

#include "Landmark.generated.h"

UCLASS()
class DRONEPORT_API ALandmark : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandmark();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite) FString _LandmarkId;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite) TArray<UStaticMesh*> _pMeshes;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite) FVector _ColliderSize;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite) FVector _ColliderRelPos;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite) UBoxComponent* _pColliderComponent;
};
