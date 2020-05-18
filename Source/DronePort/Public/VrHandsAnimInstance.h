// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "EnumHandState.h"

#include "VrHandsAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DRONEPORT_API UVrHandsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite) ENUM_HandState _HandState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ENUM_HandState _IndexState;
};
