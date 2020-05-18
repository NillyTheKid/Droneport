#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/NoExportTypes.h"

#include "StructHolder.generated.h"

class USkeletalMeshComponent;
class UArrowComponent;

USTRUCT(BlueprintType)
struct FSTRUCT_Holder {
	GENERATED_BODY()

public:
	UPROPERTY() USkeletalMeshComponent* _pHolderMesh;
	UPROPERTY() USceneComponent* _pOffset;

	FORCEINLINE bool operator==(const FSTRUCT_Holder& other) const
	{
		return (_pHolderMesh == other._pHolderMesh) && (_pOffset == other._pOffset);
	}
};