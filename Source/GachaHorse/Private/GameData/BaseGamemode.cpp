// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/BaseGamemode.h"

#include "CharactersData/BaseHorse.h"
#include "GameObjects/Checkpoints.h"

ABaseGamemode::ABaseGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;
	GEngine->AddOnScreenDebugMessage(-1,0.f,FColor::Red, FString::SanitizeFloat(Timer));

	if (!bHasStartedRun)
	{
		if (Timer >= -4.5f && !bThreeBeforeGo)
		{
			bThreeBeforeGo = true;
			Widget_ReadyToGo(3);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "3");
		}
		if (Timer >= 3.0f && !bTwoBeforeGo)
		{
			bTwoBeforeGo = true;
			Widget_ReadyToGo(2);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "2");
		}
		if (Timer >= 1.5f && !bOneBeforeGo)
		{
			bOneBeforeGo = true;
			Widget_ReadyToGo(1);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "1");
		}
		if (Timer >= 0.0f)
		{
			StartGame();
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "0");
		}
	}

	WidgetTimerUpdate(Timer);
}

void ABaseGamemode::BeginPlay()
{
	Super::BeginPlay();

	SetupWidgets();
}

void ABaseGamemode::StartGame()
{
	// INITIALIZE VALUES
	Timer = 0.0f;
	bHasStartedRun = true;

	// PLAY WIDGET
	Widget_ReadyToGo(0);

	// UNSOLIDIFY START CHECKPOINT
	if (!StartCheckpointRef->IsValidLowLevelFast())
		return;

	StartCheckpointRef->UnblockStart();
}

void ABaseGamemode::Victory()
{
	WidgetVictory(Timer);
	HorseRef->SetTargetSpeed(1);
}

void ABaseGamemode::SetupWidgets_Implementation()
{
}

void ABaseGamemode::Widget_ReadyToGo_Implementation(int TimeBeforeGo)
{
}


void ABaseGamemode::WidgetTimerUpdate_Implementation(float CurrentTimer)
{
}

void ABaseGamemode::WidgetVictory_Implementation(float CurrentTimer)
{
}