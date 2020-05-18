// Fill out your copyright notice in the Description page of Project Settings.


#include "Landmark.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALandmark::ALandmark()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add(FName("Landmark"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRoot"));
	_pColliderComponent = CreateDefaultSubobject<UBoxComponent>(FName("Collider"));

	auto rules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, false);
	_pColliderComponent->AttachToComponent(RootComponent, rules);
}

// Called when the game starts or when spawned
void ALandmark::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALandmark::OnConstruction(const FTransform& transform)
{
	if (_pColliderComponent == nullptr) return;

	_pColliderComponent->SetBoxExtent(_ColliderSize);
	_pColliderComponent->SetRelativeLocation(_ColliderRelPos);


	for (int i = 0; i < _pMeshes.Num(); i++)
	{
		auto pMeshComp = NewObject<UStaticMeshComponent>(this);
		pMeshComp->AttachTo(RootComponent);
		pMeshComp->RegisterComponent();

		pMeshComp->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		pMeshComp->SetStaticMesh(_pMeshes[i]);
	}
}

// Called every frame
void ALandmark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

