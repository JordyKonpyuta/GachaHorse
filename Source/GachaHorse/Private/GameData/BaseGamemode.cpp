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

	InstanceRef = Cast<UBaseGameInstance>(GetGameInstance());

	bIsTrainingMode = InstanceRef->bIsTrainingMode;
	BestTime = InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime;
	BestTimeSplits = InstanceRef->LevelData[InstanceRef->LevelSelected].PersonalTimeSplits;

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

	int CurrentWorld = InstanceRef->LevelSelected;
	
	for (TPair<FName, float>& CurrentRanking : InstanceRef->LevelData[CurrentWorld].Rankings)
	{
		if (TimeOfRank < CurrentRanking.Value)
			break;
		NewRank +=1;
	}

	return NewRank;
}

void ABaseGamemode::Victory()
{
	BestTime = Timer;
	if (bIsTrainingMode)
	{
		Timer = 0.0f;
		return;
	}
	bHasEndedRun = true;

	int OldRank = CalculateRank(InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime);
	RankAchieved = CalculateRank(Timer);

	int MoneyAdded = 10;

	if (RankAchieved >= OldRank)
	{
		MoneyAdded = 10 + (OldRank - RankAchieved) * 10;
		InstanceRef->AddMoney(MoneyAdded);
	}
	
	WidgetVictory(Timer, OldRank, RankAchieved, MoneyAdded);
	HorseRef->SetTargetSpeed(1);
	CheckMissions();
}

void ABaseGamemode::CheckMissions()
{
	if (InstanceRef->MissionUnavailable)
	{
		MissionsCanceled();
		return;
	}
	FTimerHandle MissionHandle;
	FirstMissionWidget(true, 100);
	int Reward = 101 - InstanceRef->RankMissionTarget * 3;
	FirstMissionWidget(true, Reward);
	FirstMissionWidget(true, 100);
	
	// FIRST : THE COURSE YOU RAN THROUGH
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::CheckFirstMission,
		0.25f,
		false);
	
	// SECOND : RANK MISSION TARGET
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::CheckSecondMission,
		0.50f,
		false);
	
	// THIRD : evil and intimidating horse
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::CheckThirdMission,
		0.75f,
		false);
}

void ABaseGamemode::CheckFirstMission()
{
	if (GetWorld() == InstanceRef->TrackMissionTarget)
	{
		InstanceRef->AddMoney(100);

		FirstMissionWidget(false, 100);
	}
}

void ABaseGamemode::CheckSecondMission()
{
	if (InstanceRef->RankMissionTarget <= RankAchieved)
	{
		int Reward = 101 - RankAchieved * 3;
		InstanceRef->AddMoney(Reward);

		SecondMissionWidget(false, Reward);
	}
}

void ABaseGamemode::CheckThirdMission()
{
	if (InstanceRef->HorseIDMissionTarget == InstanceRef->ChosenHorseData.HorseID)
	{
		InstanceRef->AddMoney(100);

		ThirdMissionWidget(false, 100);
	}
}

void ABaseGamemode::FirstMissionWidget_Implementation(bool bAnimation, int Prize)
{
}

void ABaseGamemode::SecondMissionWidget_Implementation(bool bAnimation, int Prize)
{
}

void ABaseGamemode::ThirdMissionWidget_Implementation(bool bAnimation, int Prize)
{
}

void ABaseGamemode::MissionsCanceled_Implementation()
{
}