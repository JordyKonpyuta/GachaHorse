// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/MainMenuGamemode.h"
#include "GameData/AllStructs.h"
#include "GameData/BaseGameInstance.h"

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

	FTimerHandle FuckYou;
	GetWorldTimerManager().SetTimer(
		FuckYou,
		this,
		&AMainMenuGamemode::DoWeNeedToRefreshNow,
		1.0f,
		true
		);
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
		if (Current.GetMinute() > Old.GetMinute() + 15)
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
	float TimeReturned = (NextMissionTime.GetMinute() - CurrentTime.GetMinute()) * 60.0f;
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
	if (SummonMoney < Price)
		return;
	
	TArray<int> AllGains = CalculatePossibleHorseGains(bTenSummons);
	float Delay = 0.25f;

	for (int CurrentHorseID : AllGains)
	{
		PrepareSummonResults(true, CurrentHorseID, Delay);
		InstanceRef->ObtainedHorse(CurrentHorseID);
		Delay += 0.5f;
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, FString::SanitizeFloat(Delay));
	}
}

void AMainMenuGamemode::GachaPullEquips(bool bTenSummons)
{
	// MAKE SURE WE HAVE ENOUGH MONEY (dummy :>)
	int Price = bTenSummons ? 1000 : 100;
	if (SummonMoney < Price)
		return;
	
	TArray<int> AllGains = CalculatePossibleEquipGains(bTenSummons);
	float Delay = 0.25f;
	
	for (int CurrentEquipID : AllGains)
	{
		PrepareSummonResults(false, CurrentEquipID, Delay);
		InstanceRef->ObtainedHorse(CurrentEquipID);
		Delay += 0.5f;
	}
}

void AMainMenuGamemode::PrepareSummonResults(bool bIsHorses, int ThingID, float Delay)
{
	FTimerHandle TempSummonHandle;
	FTimerDelegate TempSummonDelegate;

	TempSummonDelegate.BindUFunction(this, "DisplaySummonResults", static_cast<bool>(bIsHorses), static_cast<int>(ThingID));

	GetWorldTimerManager().SetTimer(
		TempSummonHandle,
		TempSummonDelegate,
		Delay,
		false);
}
	
void AMainMenuGamemode::DisplaySummonResults_Implementation(bool bIsHorses, int ThingID)
{
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

	if (InstanceRef->LevelData.IsEmpty())
		return;
	
	for (TPair<FName, float>& CurrentRanking : InstanceRef->LevelData[CurrentWorldSelected].Rankings)
	{
		if (!bPlayerAppeared)
		{
			if (InstanceRef->LevelData[CurrentWorldSelected].BestPersonalTime < CurrentRanking.Value)
			{
				DisplayRankingsResults(true, FName(*InstanceRef->PlayerName), InstanceRef->LevelData[CurrentWorldSelected].BestPersonalTime);
				bPlayerAppeared = true;
				DisplayRankingsResults(false, CurrentRanking.Key, CurrentRanking.Value);
			}
			else
			{
				DisplayRankingsResults(false, CurrentRanking.Key, CurrentRanking.Value);
			}
		}
		else
		{
			DisplayRankingsResults(false, CurrentRanking.Key, CurrentRanking.Value);
		}
	}
}

void AMainMenuGamemode::DisplayRankingsResults_Implementation(bool bIsPlayer, FName CurRankName, float CurRankTime)
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

	if (!AllHorsesPossessed->IsValidLowLevelFast())
	{
		UE_LOG(LogTemp, Warning, TEXT("DATA TABLE FOR HORSES NOT FOUND, SHUTTING THE GAME DOWN"));
		ErrorFound = true;
	}
	else
	{
		if (!AllHorsesPossessed->RowStruct->IsValidLowLevelFast())
		{
			UE_LOG(LogTemp, Warning, TEXT("DATA TABLE APPEARS TO BE CORRUPTED, SHUTTING THE GAME DOWN"));
			ErrorFound = true;
		}
		else
		{
			if (AllHorsesPossessed->RowStruct != FHorseDataStruct::StaticStruct())
			{
				UE_LOG(LogTemp, Warning, TEXT("DATA TABLE IS NOT OF THE CORRECT TYPE, SHUTTING THE GAME DOWN"));
				ErrorFound = true;
			}
		}
	}
	
	if (ErrorFound)
		FGenericPlatformMisc::RequestExit(false);
}
