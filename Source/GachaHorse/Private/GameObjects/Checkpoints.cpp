// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Checkpoints.h"

#include "CharactersData/BaseHorse.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACheckpoints::ACheckpoints()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SCENE COMPONENT
	SceneComp = CreateDefaultSubobject<USceneComponent>("Scene Component");
	SetRootComponent(SceneComp);

	// BOX COMPONENT
	CheckpointArea = CreateDefaultSubobject<UBoxComponent>("CheckPointArea");
	CheckpointArea->SetupAttachment(RootComponent);
	CheckpointArea->SetBoxExtent(FVector(5000,100,3000));
	CheckpointArea->SetRelativeLocation(FVector(0,0,3000));

	// ARROW
	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);
	Arrow->ArrowSize = 5.0f;
	Arrow->SetRelativeRotation(FRotator(0,-90,0));
	Arrow->SetRelativeLocation(FVector(0,0,180));
	
	CheckpointArea->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoints::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ACheckpoints::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ACheckpoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoints::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABaseHorse>(OtherActor))
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "A");
}

