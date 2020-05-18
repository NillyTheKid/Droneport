#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class ENUM_PlayerHandsState : uint8
{
	LEFT_INDEX = 0		UMETA("LeftIndex"),
	LEFT_CORE = 1		UMETA("LeftCore"),
	RIGHT_INDEX = 2		UMETA("RightIndex"),
	RIGHT_CORE = 3		UMETA("RightCore")
};