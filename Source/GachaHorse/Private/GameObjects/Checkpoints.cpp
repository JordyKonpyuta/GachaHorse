// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Checkpoints.h"

#include "CharactersData/BaseHorse.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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
	CheckpointArea->SetBoxExtent(FVector(100,5000,3000));
	CheckpointArea->SetRelativeLocation(FVector(0,0,3000));

	// ARROW
	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->SetupAttachment(RootComponent);
	Arrow->ArrowSize = 5.0f;
	Arrow->SetRelativeRotation(FRotator(0,0,0));
	Arrow->SetRelativeLocation(FVector(0,0,180));
	
	CheckpointArea->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoints::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ACheckpoints::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> AllCheckpointsFound;

	switch (CurrentCheckpointType)
	{
	case ECheckpointType::Generic:
		break;
	case ECheckpointType::LastGen:
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoints::StaticClass(), AllCheckpointsFound);
		CurrentIndex = AllCheckpointsFound.Num() - 1;
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Green, FString::FromInt(CurrentIndex));
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Yellow, FString::FromInt(AllCheckpointsFound.Num()));
		break;
	case ECheckpointType::Start:
		CurrentIndex = 0;
		// MAKE ITS COLLISIONS HARD
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "Wait wat");
	}
}



// Called every frame
void ACheckpoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckpoints::OnComponentBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<ABaseHorse>(OtherActor))
		return;

	switch (CurrentCheckpointType)
	{
	case ECheckpointType::Generic :
		RegularCheckpointCrossed(Cast<ABaseHorse>(OtherActor), false);
		break;
	case ECheckpointType::LastGen :
		RegularCheckpointCrossed(Cast<ABaseHorse>(OtherActor), false);
		break;
	case ECheckpointType::Start :
		StartCheckPointCrossed();
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "Wait wat");
	}
}

void ACheckpoints::RegularCheckpointCrossed(ABaseHorse* HorseActor, bool LastGen)
{
	GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, FString::FromInt(CurrentIndex));
	if (HorseActor->CurrentCheckpointIndex != CurrentIndex - 1)
		return;

	FVector NewRespawnLoc = HorseActor->GetActorLocation();
	NewRespawnLoc.Y = GetActorLocation().Y;
	FTransform NewRespawn = FTransform(GetActorRotation(), NewRespawnLoc,  FVector(1.0f,1.0f,1.0f));
	
	HorseActor->SetPlayerRespawn(NewRespawn);
	HorseActor->CurrentCheckpointIndex = CurrentIndex;
}

void ACheckpoints::StartCheckPointCrossed()
{
}