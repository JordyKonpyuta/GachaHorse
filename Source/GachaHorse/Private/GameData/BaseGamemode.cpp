// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/BaseGamemode.h"

#include "CharactersData/BaseController.h"
#include "CharactersData/BaseHorse.h"
#include "GameData/BaseGameInstance.h"
#include "GameObjects/Checkpoints.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialExpressionLocalPosition.h"

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
		}
		if (Timer >= -4.0f && !bTwoBeforeGo)
		{
			bTwoBeforeGo = true;
			Widget_ReadyToGo(2);
		}
		if (Timer >= -2.0f && !bOneBeforeGo)
		{
			bOneBeforeGo = true;
			Widget_ReadyToGo(1);
		}
		if (Timer >= 0.0f)
		{
			StartGame();
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
	
	if (CheckpointIndex != 0)
	{
		int AllIndexes = CheckpointIndex - 1;
		float TimeAdded = Timer;
		while (AllIndexes >= 0)
		{
			TimeAdded -= CurrentTimeSplits[AllIndexes];
			AllIndexes -= 1;
		}
		CurrentTimeSplits.Add(TimeAdded);
	}
	else
	{
		CurrentTimeSplits.Add(Timer);
	}
	
	bool bNoBestTimeLocated = true;
	if (BestTimeSplits.IsValidIndex(CheckpointIndex))
	{
		TimeDifference = CurrentTimeSplits[CheckpointIndex] - BestTimeSplits[CheckpointIndex];
		bNoBestTimeLocated = false;
	}
	
	if (bNoBestTimeLocated)
		return;
	
	DisplaySplitWidget(TimeDifference);
}

void ABaseGamemode::DisplaySplitFinal()
{
	float TimeDifference = 0.0f;
	if (BestTime > 0)
		TimeDifference = BestTime - Timer;
	
	DisplaySplitWidget(TimeDifference);
}

	// =========================
	// ==       Widgets       ==
	// =========================

void ABaseGamemode::SetupWidgets_Implementation()
{
}

void ABaseGamemode::DisplaySplitWidget_Implementation(float TimeChange)
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
	bDoNotRunTimer = true;

	int OldRank = CalculateRank(InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime);
	RankAchieved = CalculateRank(Timer);

	int MoneyAdded = 50;

	if (RankAchieved < OldRank)
	{
		MoneyAdded = 50 + (OldRank - RankAchieved) * 50;
		InstanceRef->AddMoney(MoneyAdded);
	}

	if (BestTime < InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime)
	{
		InstanceRef->LevelData[InstanceRef->LevelSelected].BestPersonalTime = BestTime;
		InstanceRef->LevelData[InstanceRef->LevelSelected].PersonalTimeSplits = CurrentTimeSplits;
	}

	Cast<ABaseController>(GetWorld()->GetFirstPlayerController())->ActivateTouchInterface(nullptr);
	
	WidgetVictory(Timer, OldRank, RankAchieved, MoneyAdded);
	HorseRef->FinishRace();
	CheckMissions();
}

void ABaseGamemode::TrainingModeVictory()
{
	DisplaySplitFinal();
	if (Timer < BestTime)
	{
		BestTime = Timer;
		BestTimeSplits = CurrentTimeSplits;
	}
	CurrentTimeSplits.Empty();
	
	Timer = 0.0f;
}

void ABaseGamemode::CheckMissions()
{
	if (InstanceRef->MissionUnavailable)
	{
		MissionsCanceled();
		return;
	}
	FTimerHandle MissionHandle;
	FirstMissionWidget(false, 200);
	SecondMissionWidget(false, (101 - InstanceRef->RankMissionTarget) * 6);
	ThirdMissionWidget(false, 200);

	// MISSION 1
	if (InstanceRef->LevelData[InstanceRef->LevelSelected].WorldToLoad == InstanceRef->TrackMissionTarget)
	{
		InstanceRef->AddMoney(200);
	}

	// MISSION 2
	if (RankAchieved <= InstanceRef->RankMissionTarget)
	{
		InstanceRef->AddMoney((101 - InstanceRef->RankMissionTarget) * 6);
	}
	
	// MISSION 3
	if (InstanceRef->HorseIDMissionTarget == InstanceRef->ChosenHorseData.HorseID)
	{
		InstanceRef->AddMoney(200);
	}
	
	// FIRST : THE COURSE YOU RAN THROUGH
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::AnimFirstMission,
		3.0f,
		false);
}

void ABaseGamemode::AnimFirstMission()
{
	FTimerHandle MissionHandle;

	FirstMissionWidget(true, 200);
	
	// SECOND : RANK MISSION TARGET
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::AnimSecondMission,
		0.25f,
		false);
}

void ABaseGamemode::AnimSecondMission()
{
	FTimerHandle MissionHandle;

	SecondMissionWidget(true, (101 - InstanceRef->RankMissionTarget) * 6);
	
	// THIRD : evil and intimidating horse
	GetWorldTimerManager().SetTimer(
		MissionHandle,
		this,
		&ABaseGamemode::AnimThirdMission,
		0.25f,
		false);
}

void ABaseGamemode::AnimThirdMission()
{
	ThirdMissionWidget(true, 200);
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