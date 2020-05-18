// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaSlider.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
APdaSlider::APdaSlider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Default Root");
	_pColliderComp = CreateDefaultSubobject<UBoxComponent>("Collider");
	_pSliderDecalComp = CreateDefaultSubobject<UDecalComponent>("Slider Decal");
	_pBarDecalComp = CreateDefaultSubobject<UDecalComponent>("Bar Decal");
	_pMovableComp = CreateDefaultSubobject<USceneComponent>("Movable");
	_pBarLeft = CreateDefaultSubobject<USceneComponent>("Bar Left");
	_pBarRight = CreateDefaultSubobject<USceneComponent>("Bar Right");

	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pBarDecalComp->AttachToComponent(RootComponent, rules);
	_pMovableComp->AttachToComponent(RootComponent, rules);
	_pColliderComp->AttachToComponent(_pMovableComp, rules);
	_pSliderDecalComp->AttachToComponent(_pMovableComp, rules);
	_pBarLeft->AttachToComponent(_pBarDecalComp, rules);
	_pBarRight->AttachToComponent(_pBarDecalComp, rules);

	_pColliderComp->OnComponentBeginOverlap.AddDynamic(this, &APdaSlider::OnOverlapBegin);
	_pColliderComp->OnComponentEndOverlap.AddDynamic(this, &APdaSlider::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APdaSlider::BeginPlay()
{
	Super::BeginPlay();
	
	_pColliderComp->SetBoxExtent(colliderDims);

	_pSliderDecalComp->SetDecalMaterial(pSliderDecalMat);
	_pSliderDecalComp->DecalSize = sliderDecalSize;
	_pSliderDecalComp->SetSortOrder(2);

	_pBarDecalComp->SetDecalMaterial(pBarDecalMat);
	_pBarDecalComp->DecalSize = FVector(barHeight, barWidth, 1);
	_pBarDecalComp->SetSortOrder(1);

	_pBarLeft->SetRelativeLocation(FVector(0, barWidth, 0));
	_pBarRight->SetRelativeLocation(FVector(0, -barWidth, 0));

	_BarWidthDist = FVector::Distance(_pBarLeft->K2_GetComponentLocation(), _pBarRight->K2_GetComponentLocation());
	Reset();
}

void APdaSlider::OnConstruction(const FTransform& transform)
{
	_pColliderComp->SetBoxExtent(colliderDims);

	_pSliderDecalComp->SetDecalMaterial(pSliderDecalMat);
	_pSliderDecalComp->DecalSize = sliderDecalSize;
	_pSliderDecalComp->SetSortOrder(2);

	_pBarDecalComp->SetDecalMaterial(pBarDecalMat);
	_pBarDecalComp->DecalSize = FVector(barHeight, barWidth, 1);
	_pBarDecalComp->SetSortOrder(1);

	_pBarLeft->SetRelativeLocation(FVector(0, barWidth, 0));
	_pBarRight->SetRelativeLocation(FVector(0, -barWidth, 0));

	_BarWidthDist = FVector::Distance(_pBarLeft->K2_GetComponentLocation(), _pBarRight->K2_GetComponentLocation());
	Reset();
}

// Called every frame
void APdaSlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_pFingerComp != nullptr)
	{
		FHitResult result;
		auto closestPoint = FMath::ClosestPointOnLine(_pBarLeft->K2_GetComponentLocation(), _pBarRight->K2_GetComponentLocation(), 
			_pFingerComp->K2_GetComponentLocation());

		_pMovableComp->K2_SetWorldLocation(closestPoint, false, result, false);
	}
}

UFUNCTION(BlueprintCallable) float APdaSlider::GetSliderPercentage()
{
	auto dist = FVector::Distance(_pBarLeft->K2_GetComponentLocation(), _pSliderDecalComp->K2_GetComponentLocation());
	return (dist / _BarWidthDist);
}

UFUNCTION() void APdaSlider::Reset()
{
	float value;
	if (startValue > 1) //User set it to value out of 255
	{
		value = startValue / 255.f;
	}
	else //User set it to value out of 1 or to 0
	{
		value = startValue;
	}

	float dist = value * _BarWidthDist;
	FHitResult result;
	_pMovableComp->K2_SetWorldLocation(_pBarLeft->K2_GetComponentLocation() + (-_pBarLeft->GetRightVector() * dist), false, result, false);
}

UFUNCTION() void APdaSlider::OnOverlapBegin(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx, bool isFromSweep, const FHitResult& sweepResult)
{
	if (pOtherComp->ComponentHasTag("Finger"))
	{
		_pFingerComp = ((UBoxComponent*)pOtherComp);
	}
}

UFUNCTION() void APdaSlider::OnOverlapEnd(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx)
{
	if (pOtherComp == _pFingerComp)
	{
		_pFingerComp = nullptr;
	}
}

