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

	if (!bGameLoaded)
	{
		CheckAllAssets();
	}

	if (!bDoNotRunTimer)
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

	PrepareGame();

	SetupWidgets();
}
	
	// ==========================
	// ==      Beginnings      ==
	// ==========================

void ABaseGamemode::PrepareGame_Implementation()
{
}

void ABaseGamemode::CheckAllAssets()
{
	int32 RemainingRequests = FStreamingManagerCollection::Get().BlockTillAllRequestsFinished(0.0f, true);

	if (RemainingRequests == 0)
	{
		bGameLoaded = true;
		GamePrepared();
	}
}

void ABaseGamemode::GamePrepared()
{
	bDoNotRunTimer = false;
	GamePrepared_Blueprint();
}

void ABaseGamemode::GamePrepared_Blueprint_Implementation()
{
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
	
	// =========================
	// ==        Timer        ==
	// =========================

void ABaseGamemode::DisplaySplit(int CheckpointIndex)
{
	float TimeDifference = 0.0f;
	if (BestTimeSplits.IsValidIndex(CheckpointIndex))
		TimeDifference = BestTimeSplits[CheckpointIndex] - Timer;
	
	if (CheckpointIndex != 0)
	{
		CurrentTimeSplits.Add(Timer - CurrentTimeSplits[CheckpointIndex - 1]);
	}
	else
	{
		CurrentTimeSplits.Add(Timer);
	}
	
	DisplaySplitWidget(TimeDifference);
}

	// =========================
	// ==       Widgets       ==
	// =========================

void ABaseGamemode::SetupWidgets_Implementation()
{
}

void ABaseGamemode::DisplaySplitWidget_Implementation(float Difference)
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
	bDoNotRunTimer = true;

	int OldRank = CalculateRank(InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime);
	RankAchieved = CalculateRank(Timer);

	int MoneyAdded = 10;

	if (RankAchieved < OldRank)
	{
		MoneyAdded = 10 + (OldRank - RankAchieved) * 10;
		InstanceRef->AddMoney(MoneyAdded);
		InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime = BestTime;
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
	FirstMissionWidget(false, 100);
	int Reward = 101 - InstanceRef->RankMissionTarget * 3;
	SecondMissionWidget(false, Reward);
	ThirdMissionWidget(false, 100);
	
	// FIRST : THE COURSE YOU RAN THROUGH
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::CheckFirstMission,
		1.25f,
		false);
}

void ABaseGamemode::CheckFirstMission()
{
	GEngine->AddOnScreenDebugMessage(-1, 500.f, FColor::Blue, FString::FromInt(InstanceRef->LevelSelected));
	if (InstanceRef->LevelData[InstanceRef->LevelSelected].WorldToLoad == InstanceRef->TrackMissionTarget)
	{
		InstanceRef->AddMoney(100);

		FirstMissionWidget(true, 100);
	}
	FTimerHandle MissionHandle;
	
	// SECOND : RANK MISSION TARGET
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::CheckSecondMission,
		0.25f,
		false);
}

void ABaseGamemode::CheckSecondMission()
{
	if (RankAchieved <= InstanceRef->RankMissionTarget)
	{
		int Reward = 101 - RankAchieved * 3;
		InstanceRef->AddMoney(Reward);

		SecondMissionWidget(true, Reward);
	}
	FTimerHandle MissionHandle;
	
	// THIRD : evil and intimidating horse
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::CheckThirdMission,
		0.25f,
		false);
}

void ABaseGamemode::CheckThirdMission()
{
	if (InstanceRef->HorseIDMissionTarget == InstanceRef->ChosenHorseData.HorseID)
	{
		InstanceRef->AddMoney(100);

		ThirdMissionWidget(true, 100);
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