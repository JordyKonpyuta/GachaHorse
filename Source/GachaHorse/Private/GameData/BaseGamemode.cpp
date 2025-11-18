// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/BaseGamemode.h"

#include "CharactersData/BaseHorse.h"
#include "GameData/BaseGameInstance.h"
#include "GameObjects/Checkpoints.h"

	// ==========================
	// ==    Base Functions    ==
	// ==========================

ABaseGamemode::ABaseGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasEndedRun)
		Timer += DeltaTime;

	if (!bHasStartedRun)
	{
		if (Timer >= -6.0f && !bThreeBeforeGo)
		{
			bThreeBeforeGo = true;
			Widget_ReadyToGo(3);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "3");
		}
		if (Timer >= -4.0f && !bTwoBeforeGo)
		{
			bTwoBeforeGo = true;
			Widget_ReadyToGo(2);
			GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, "2");
		}
		if (Timer >= -2.0f && !bOneBeforeGo)
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

	BaseGameInstanceRef = Cast<UBaseGameInstance>(GetGameInstance());

	SetupWidgets();
}
	
	// ==========================
	// ==      Beginnings      ==
	// ==========================

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
	
	// =========================
	// ==       Widgets       ==
	// =========================

void ABaseGamemode::SetupWidgets_Implementation()
{
}

void ABaseGamemode::HideAwayTimer_Implementation()
{
}

void ABaseGamemode::Widget_ReadyToGo_Implementation(int TimeBeforeGo)
{
}


void ABaseGamemode::WidgetTimerUpdate_Implementation(float CurrentTimer)
{
}

void ABaseGamemode::WidgetVictory_Implementation(float CurrentTimer, int OldRank, int NewRank, int RewardAmount)
{
}
	
	// =========================
	// ==       EndGame       ==
	// =========================

int ABaseGamemode::CalculateRank(float TimeOfRank) const
{
	int NewRank = 1;

	int CurrentWorld = BaseGameInstanceRef->LevelSelected;
	
	for (TPair<FName, float>& CurrentRanking : BaseGameInstanceRef->LevelData[CurrentWorld].Rankings)
	{
		if (TimeOfRank < CurrentRanking.Value)
			break;
		NewRank +=1;
	}

	return NewRank;
}

void ABaseGamemode::Victory()
{
	bHasEndedRun = true;

	int OldRank = CalculateRank(BaseGameInstanceRef->LevelData[BaseGameInstanceRef->LevelSelected].BestPersonalTime);
	int NewRank = CalculateRank(Timer);

	int MoneyAdded = 10;

	if (NewRank >= OldRank)
	{
		MoneyAdded = 10 + (OldRank - NewRank) * 10;
		BaseGameInstanceRef->AddMoney(MoneyAdded);
	}
	
	WidgetVictory(Timer, OldRank, NewRank, MoneyAdded);
	HorseRef->SetTargetSpeed(1);
}