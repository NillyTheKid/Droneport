#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class ENUM_PlayerHandPart : uint8
{
	INDEX		UMETA(DisplayName = "INDEX"),
	CORE		UMETA(DisplayName = "CORE")
};