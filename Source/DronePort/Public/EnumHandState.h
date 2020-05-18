#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class ENUM_HandState : uint8
{
	OPEN		UMETA(DisplayName = "OPEN"),
	CLOSED		UMETA(DisplayName = "CLOSED")
};