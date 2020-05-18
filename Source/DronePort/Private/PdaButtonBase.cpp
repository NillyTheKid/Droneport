// Fill out your copyright notice in the Description page of Project Settings.


#include "PdaButtonBase.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
APdaButtonBase::APdaButtonBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Components
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	_pColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	_pDecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));

	//Attach Components
	FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pColliderComp->AttachToComponent(RootComponent, rules);
	_pDecalComp->AttachToComponent(RootComponent, rules);

	//Bind Overlap functions
	_pColliderComp->OnComponentBeginOverlap.AddDynamic(this, &APdaButtonBase::OnOverlapBegin);
	_pColliderComp->OnComponentEndOverlap.AddDynamic(this, &APdaButtonBase::OnOverlapEnd);
}

UFUNCTION(BlueprintCallable) void APdaButtonBase::SetMaterial(UMaterialInterface* pMat)
{
	_pDecalComp->SetDecalMaterial(pMat);
}

// Called when the game starts or when spawned
void APdaButtonBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APdaButtonBase::OnConstruction(const FTransform& transform)
{
	_pColliderComp->SetBoxExtent(colliderDims);
	_pDecalComp->SetDecalMaterial(pDecalMat);
	_pDecalComp->DecalSize = decalSize;
}

void APdaButtonBase::OnPress()
{
}

void APdaButtonBase::OnRelease()
{
}

// Called every frame
void APdaButtonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UFUNCTION() void APdaButtonBase::OnOverlapBegin(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx, bool isFromSweep, const FHitResult& sweepResult)
{
	if (pOtherComp->ComponentHasTag("Finger"))
	{
		OnPress();
	}
}

UFUNCTION() void APdaButtonBase::OnOverlapEnd(UPrimitiveComponent* pComp, AActor* pOtherActor, UPrimitiveComponent* pOtherComp, int32 otherBodyIdx)
{
	if (pOtherComp->ComponentHasTag("Finger"))
	{
		OnRelease();
	}
}

