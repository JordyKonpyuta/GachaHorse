// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/ObstacleBase.h"

#include "CharactersData/BaseHorse.h"
#include "Components/BoxComponent.h"

// Sets default values
AObstacleBase::AObstacleBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// SCENE COMPONENT
	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene Component");
	SetRootComponent(SceneComp);

	// BOX COMPONENT
	RagdollArea = CreateDefaultSubobject<UBoxComponent>("CheckPointArea");
	RagdollArea->SetupAttachment(RootComponent);
	RagdollArea->SetBoxExtent(FVector(5000,5000,30));
	RagdollArea->SetRelativeLocation(FVector(0,0,0));

	// MESH COMPONENT
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Overlap);

	RagdollArea->OnComponentBeginOverlap.AddDynamic(this, &AObstacleBase::OnComponentBeginOverlap);
	RagdollArea->OnComponentEndOverlap.AddDynamic(this, &AObstacleBase::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AObstacleBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AObstacleBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bMercyTimerRuns)
		MercyTimer += DeltaTime;

	if (MercyTimer > MercyTimerLimit)
	{
		MercyTimer = 0.0f;
		bMercyTimerRuns = false;
		if (HorseRef->IsValidLowLevelFast())
			HorseRef->BeginRagdoll();
	}
}

void AObstacleBase::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ABaseHorse>(OtherActor))
		return;

	bMercyTimerRuns = true;
	HorseRef = Cast<ABaseHorse>(OtherActor);
}

void AObstacleBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	MercyTimer = 0.0f;
	bMercyTimerRuns = false;
}

