// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Checkpoints.h"
#include "CharactersData/BaseHorse.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameData/BaseGamemode.h"
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
		CurrentIndex = AllCheckpointsFound.Num() - 2;
		break;
	case ECheckpointType::Start:
		CurrentIndex = 0;
		
		if (!Cast<ABaseGamemode>(GetWorld()->GetAuthGameMode()))
			break;
		
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoints::StaticClass(), AllCheckpointsFound);
		CurrentIndex = AllCheckpointsFound.Num() - 2;
		if (Cast<ABaseGamemode>(GetWorld()->GetAuthGameMode()))
			Cast<ABaseGamemode>(GetWorld()->GetAuthGameMode())->StartCheckpointRef = this;

		CheckpointArea->SetCollisionResponseToAllChannels(ECR_Block);
		
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

	ABaseHorse* OurHorse = Cast<ABaseHorse>(OtherActor);

	switch (CurrentCheckpointType)
	{
	case ECheckpointType::Generic :
		RegularCheckpointCrossed(OurHorse);
		break;
	case ECheckpointType::LastGen :
		RegularCheckpointCrossed(OurHorse);
		break;
	case ECheckpointType::Start :
		StartCheckPointCrossed(OurHorse);
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "Wait wat");
	}
}

void ACheckpoints::UnblockStart()
{
	CheckpointArea->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ACheckpoints::RegularCheckpointCrossed(ABaseHorse* HorseActor)
{
	GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, FString::FromInt(CurrentIndex));
	if (HorseActor->CurrentCheckpointIndex != CurrentIndex - 1)
		return;

	FTransform NewRespawn = FTransform(GetActorRotation(), GetActorLocation(),  FVector(1.0f,1.0f,1.0f));
	
	HorseActor->SetPlayerRespawn(NewRespawn);
	HorseActor->CurrentCheckpointIndex = CurrentIndex;
}

void ACheckpoints::StartCheckPointCrossed(ABaseHorse* HorseActor)
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, FString::FromInt(HorseActor->CurrentCheckpointIndex ));
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, FString::FromInt(CurrentIndex));
	if (HorseActor->CurrentCheckpointIndex != CurrentIndex)
		return;

	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "WIN");
}
