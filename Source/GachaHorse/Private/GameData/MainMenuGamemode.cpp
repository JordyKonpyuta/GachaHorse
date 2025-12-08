// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/MainMenuGamemode.h"

#include "CharactersData/BaseController.h"
#include "CharactersData/MainMenuController.h"
#include "GameData/AllStructs.h"
#include "GameData/BaseGameInstance.h"
#include "GameObjects/MainMenuCameraManager.h"

AMainMenuGamemode::AMainMenuGamemode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainMenuGamemode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainMenuGamemode::BeginPlay()
{
	Super::BeginPlay();

	InstanceRef = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	
	CheckPossibleCrash();

	FTimerHandle InitialMissionTimerHandle;
	GetWorldTimerManager().SetTimer(
		InitialMissionTimerHandle,
		this,
		&AMainMenuGamemode::DoWeNeedToRefreshNow,
		1.0f,
		false
		);
}
	
	// =========================
	// ==   Horse Selection   ==
	// =========================

void AMainMenuGamemode::SwitchHorse(bool bClickedLeft)
{
	InstanceRef->CheckAvailableHorse(bClickedLeft);
}

void AMainMenuGamemode::SelectHorseWidget_Implementation(FHorseDataStruct HorseDisplayed)
{
}


void AMainMenuGamemode::LevelUpHorse()
{
	InstanceRef->LevelUpHorse();
}

void AMainMenuGamemode::LevelUpHorseWidgetUpdate_Implementation(FHorseDataStruct HorseDisplayed)
{
}

	// =========================
	// == Equipment Selection ==
	// =========================

void AMainMenuGamemode::SwitchEquip(bool bClickedLeft)
{
	InstanceRef->CheckAvailableEquip(bClickedLeft);
}

void AMainMenuGamemode::SelectEquipWidget_Implementation(FEquipDataStruct EquipDisplayed)
{
}

void AMainMenuGamemode::LevelUpEquip()
{
	InstanceRef->LevelUpEquip();
}

void AMainMenuGamemode::LevelUpEquipWidgetUpdate_Implementation(FEquipDataStruct EquipDisplayed)
{
}
	
	// =========================
	// ==  General Selection  ==
	// =========================

void AMainMenuGamemode::UpdateViewedData()
{
	for (FHorseDataStruct ThisHorse : InstanceRef->HorseData)
	{
		if (InstanceRef->ViewedHorseData.HorseName == ThisHorse.HorseName)
		{
			GEngine->AddOnScreenDebugMessage(-1,50.f, FColor::Red, "LAAAAAAAA");
			if (InstanceRef->ViewedHorseData.ShardNumber != ThisHorse.ShardNumber)
			{
			GEngine->AddOnScreenDebugMessage(-1,50.f, FColor::Red, "DETROIT SMASH !!!!!!");
				InstanceRef->ViewedHorseData = ThisHorse;
				
			}
			break;
		}
	}
}

	// ==========================
	// ==       Missions       ==
	// ==========================

void AMainMenuGamemode::DoWeNeedToRefreshNow()
{
	FDateTime Current = FDateTime::Now();
	FDateTime Old = InstanceRef->MissionNextResetRef;

	bool bRefreshNow = false;
	if (Current.GetYear() != Old.GetYear() || Current.GetMonth() != Old.GetMonth() || Current.GetDay() != Old.GetDay() || Current.GetHour() != Old.GetHour())
		bRefreshNow = true;
	else
		if (Current.GetMinute() > Old.GetMinute())
			bRefreshNow = true;

	if (bRefreshNow)
		RefreshMissions();
	else
		PrepareDisplayMissions();
		
}

void AMainMenuGamemode::RefreshMissions()
{
	int MinuteCadran = 0;

	if (FDateTime::Now().GetMinute() < 15)
		MinuteCadran = 15;
	else if (FDateTime::Now().GetMinute() < 30)
		MinuteCadran = 30;
	else if (FDateTime::Now().GetMinute() < 45)
		MinuteCadran = 45;
	else if (FDateTime::Now().GetMinute() < 60)
		MinuteCadran = 0;

	FDateTime NewTime = FDateTime(FDateTime::Now().GetYear(), FDateTime::Now().GetMonth(),
		FDateTime::Now().GetDay(), MinuteCadran == 0 ? FDateTime::Now().GetHour()+1 : FDateTime::Now().GetHour(),
		MinuteCadran, 0, 0);

	InstanceRef->MissionNextResetRef = NewTime;
	GetWorldTimerManager().SetTimer(
		MissionTimerHandle,
		this,
		&AMainMenuGamemode::RefreshMissions,
		CalculateTimeBeforeNextRefresh(NewTime),
		false);

	// MAKE THE ACTUAL MISSIONS
	InstanceRef->PrepareMissions();
}

float AMainMenuGamemode::CalculateTimeBeforeNextRefresh(FDateTime NextMissionTime)
{
	FDateTime CurrentTime = FDateTime::Now();
	int NextMissionMinute = NextMissionTime.GetMinute() == 0 ? 60 : NextMissionTime.GetMinute();
	
	float TimeReturned = (NextMissionMinute - CurrentTime.GetMinute()) * 60.0f;
	TimeReturned -= CurrentTime.GetSecond();
	TimeReturned -= CurrentTime.GetMillisecond() / 1000.0f;
	
	return TimeReturned;
}

void AMainMenuGamemode::PrepareDisplayMissions()
{
	FString TrackName;
	for (FWorldMapDataStruct CurMapCheck : InstanceRef->LevelData)
	{
		if (CurMapCheck.WorldToLoad == InstanceRef->TrackMissionTarget)
		{
			TrackName = CurMapCheck.LevelName;
			break;
		}
	}

	GetWorldTimerManager().SetTimer(
		MissionTimerHandle,
		this,
		&AMainMenuGamemode::RefreshMissions,
		CalculateTimeBeforeNextRefresh(InstanceRef->MissionNextResetRef),
		false);
	
	FString HorseName = InstanceRef->HorseData[InstanceRef->HorseIDMissionTarget].HorseName;
	
	DisplayMissionsWidget(TrackName, InstanceRef->RankMissionTarget, HorseName);
}

	// ==========================
	// ==   GACHA! GAMBLING!   ==
	// ==========================

TArray<int> AMainMenuGamemode::CalculatePossibleHorseGains(bool bTenSummons)
{
	TArray<int> AllHorses;
	int CurOdd;

	if (!bTenSummons)
	{
		CurOdd = FMath::RandRange(0.000f, 100.000f);

		if (CurOdd >= 93.75)
		{
			// RARE HORSE :DDD
			AllHorses.Add(FMath::RandRange(9, 12));
		}
		else if (CurOdd >= 62.5)
		{
			// UNCOMMON HORSE :)
			AllHorses.Add(FMath::RandRange(5, 8));
		}
		else
		{
			// COMMON HORSE :(
			AllHorses.Add(FMath::RandRange(0, 4));
		}

		return AllHorses;
	}

	for (int i = 0; i < 9; i++)
	{
		CurOdd = FMath::RandRange(0.000f, 100.000f);

		if (CurOdd >= 93.75)
		{
			// RARE HORSE :DDD
			AllHorses.Add(FMath::RandRange(9, 12));
		}
		else if (CurOdd >= 62.5)
		{
			// UNCOMMON HORSE :)
			AllHorses.Add(FMath::RandRange(5, 8));
		}
		else
		{
			// COMMON HORSE :(
			AllHorses.Add(FMath::RandRange(0, 4));
		}
	}

	CurOdd = FMath::RandRange(0, 5);

	if (CurOdd < 5)
	{
		// UNCOMMON HORSE DDD:
		AllHorses.Add(FMath::RandRange(5, 8));
	}
	else
	{
		// RARE HORSE :DDD
		AllHorses.Add(FMath::RandRange(9, 12));
	}
	
	return AllHorses;
}

TArray<int> AMainMenuGamemode::CalculatePossibleEquipGains(bool bTenSummons)
{
	TArray<int> AllEquips;
	int CurOdd;

	if (!bTenSummons)
	{
		CurOdd = FMath::RandRange(0.000f, 100.000f);

		if (CurOdd >= 93.75)
		{
			// RARE HORSE :DDD
			AllEquips.Add(FMath::RandRange(6, 8));
		}
		else if (CurOdd >= 62.5)
		{
			// UNCOMMON HORSE :)
			AllEquips.Add(FMath::RandRange(3, 5));
		}
		else
		{
			// COMMON HORSE :(
			AllEquips.Add(FMath::RandRange(0, 2));
		}

		return AllEquips;
	}

	for (int i = 0; i < 9; i++)
	{
		CurOdd = FMath::RandRange(0.000f, 100.000f);

		if (CurOdd >= 93.75)
		{
			// RARE HORSE :DDD
			AllEquips.Add(FMath::RandRange(6, 8));
		}
		else if (CurOdd >= 62.5)
		{
			// UNCOMMON HORSE :)
			AllEquips.Add(FMath::RandRange(3, 5));
		}
		else
		{
			// COMMON HORSE :(
			AllEquips.Add(FMath::RandRange(0, 2));
		}
	}

	CurOdd = FMath::RandRange(0, 5);

	if (CurOdd < 5)
	{
		// UNCOMMON HORSE DDD:
		AllEquips.Add(FMath::RandRange(3, 5));
	}
	else
	{
		// RARE HORSE :DDD
		AllEquips.Add(FMath::RandRange(6, 8));
	}
	
	return AllEquips;
}

void AMainMenuGamemode::GachaPullHorses(bool bTenSummons)
{
	// MAKE SURE WE HAVE ENOUGH MONEY (baka :>)
	int Price = bTenSummons ? 1000 : 100;
	if (InstanceRef->GetMoney() < Price)
		return;
	
	InstanceRef->AddMoney(-Price);
	InstanceRef->AllHorsesPulled.Empty();
	
	TArray<int> AllGains = CalculatePossibleHorseGains(bTenSummons);
	float Delay = 0.25f;
	for (int i = 0; i < AllGains.Num(); i++ )
	{
		Delay +=0.5f;
		PrepareSummonResults(true, i, Delay);
		InstanceRef->ObtainedHorse(AllGains[i]);
	}
}

void AMainMenuGamemode::GachaPullEquips(bool bTenSummons)
{
	// MAKE SURE WE HAVE ENOUGH MONEY (dummy :>)
	int Price = bTenSummons ? 1000 : 100;
	if (InstanceRef->GetMoney() < Price)
		return;

	InstanceRef->AddMoney(-Price);
	InstanceRef->AllEquipsPulled.Empty();
	
	TArray<int> AllGains = CalculatePossibleEquipGains(bTenSummons);
	float Delay = 0.25f;

	for (int i = 0; i < AllGains.Num(); i++ )
	{
		Delay += 0.5f;
		PrepareSummonResults(false, i, Delay);
		InstanceRef->ObtainedEquip(AllGains[i]);
	}
}

void AMainMenuGamemode::PrepareSummonResults(bool bIsHorses, int Index, float Delay)
{
	FTimerHandle TempSummonHandle;
	FTimerDelegate TempSummonDelegate;

	TempSummonDelegate.BindUFunction(this, "DisplaySummonResults", static_cast<bool>(bIsHorses), static_cast<int>(Index));

	GetWorldTimerManager().SetTimer(
		TempSummonHandle,
		TempSummonDelegate,
		Delay,
		false);
}
	
void AMainMenuGamemode::DisplaySummonResults_Implementation(bool bIsHorses, int Index)
{
}

	// ==========================
	// ==    Loading Screen    ==
	// ==========================

void AMainMenuGamemode::PrepareLoadingScreen()
{
	CreateLoadingScreen_Widget();
	
	FTimerHandle LoadingTimer;
	GetWorldTimerManager().SetTimer(
		LoadingTimer,
		this,
		&AMainMenuGamemode::RemoveLoadingScreen,
		1.5f,
		false);
}

void AMainMenuGamemode::CreateLoadingScreen_Widget_Implementation()
{
}

void AMainMenuGamemode::RemoveLoadingScreen_Implementation()
{
}

void AMainMenuGamemode::LoadingScreenDone()
{
	Cast<AMainMenuController>(GetWorld()->GetFirstPlayerController())->CameraHandler->ChangeCameraView();
}

	// ==========================
	// ==    Fission Mailed    ==
	// ==========================

void AMainMenuGamemode::DisplayMissionsWidget_Implementation(FString& TrackMissionName, int RankMissionNumber,
	FString& HorseMissionName)
{
}
	
	// ==========================
	// ==   Rankings Results   ==
	// ==========================


void AMainMenuGamemode::PrepareRankingResults(int CurrentWorldSelected)
{
	bool bPlayerAppeared = false;
	int CurRank = 1;

	if (InstanceRef->LevelData.IsEmpty())
		return;
	
	for (TPair<FName, float>& CurrentRanking : InstanceRef->LevelData[CurrentWorldSelected].Rankings)
	{
		if (!bPlayerAppeared)
		{
			if (InstanceRef->LevelData[CurrentWorldSelected].BestPersonalTime < CurrentRanking.Value)
			{
				DisplayRankingsResults(true, FName(*InstanceRef->PlayerName), InstanceRef->LevelData[CurrentWorldSelected].BestPersonalTime, CurRank);
				bPlayerAppeared = true;
				CurRank++;
				DisplayRankingsResults(false, CurrentRanking.Key, CurrentRanking.Value, CurRank);
			}
			else
			{
				DisplayRankingsResults(false, CurrentRanking.Key, CurrentRanking.Value, CurRank);
			}
		}
		else
		{
			DisplayRankingsResults(false, CurrentRanking.Key, CurrentRanking.Value, CurRank);
		}
		CurRank++;
	}

	if (!bPlayerAppeared)
		DisplayRankingsResults(true, FName(*InstanceRef->PlayerName), InstanceRef->LevelData[CurrentWorldSelected].BestPersonalTime, CurRank);
}

void AMainMenuGamemode::DisplayRankingsResults_Implementation(bool bIsPlayer, FName CurRankName, float CurRankTime, int CurrentRank)
{
}


	// ==========================
	// ==        DEBOOG        ==
	// ==========================

void AMainMenuGamemode::CheckPossibleCrash()
{
	bool ErrorFound = false;

	if (!InstanceRef->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("INSTANCEREF IS NOT SET, SHUTTING THE GAME DOWN"));
		ErrorFound = true;
	}
	
	if (ErrorFound)
		FGenericPlatformMisc::RequestExit(false);
}
