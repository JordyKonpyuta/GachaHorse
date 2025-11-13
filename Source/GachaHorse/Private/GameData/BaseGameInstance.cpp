// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/BaseGameInstance.h"

#include "GameData/AllStructs.h"
#include "GameData/HorseGameSave.h"
#include "Kismet/GameplayStatics.h"

	// ==========================
	// ==    Base Functions    ==
	// ==========================

UBaseGameInstance::UBaseGameInstance()
{
	
}

void UBaseGameInstance::CheckSaves()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		SaveGameRef = Cast<UHorseGameSave>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
	}
	else
	{
		SaveGameRef = Cast<UHorseGameSave>(UGameplayStatics::CreateSaveGameObject(UHorseGameSave::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveName, 0);
		InitializeFirstSave();
	}
}

TArray<int> UBaseGameInstance::CalculateHorseGains(bool bTenSummons)
{
	TArray<int> AllHorses;
	int CurOdd = 0;

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

// =========================
	// ==     Actual Game     ==
	// =========================

void UBaseGameInstance::StartGame(TSoftObjectPtr<UWorld> WorldToLoad, int HorseID, int EquipmentID)
{
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == HorseID)
		{
			ChosenHorseData = HorseData[i];
			break;
		}
	}
	for (int i = 0; i < EquipData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == EquipmentID)
		{
			ChosenEquipData = EquipData[i];
			break;
		}
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),WorldToLoad);
}

	// =========================
	// ==   Saves Functions   ==
	// =========================

void UBaseGameInstance::InitializeFirstSave()
{
	for (FName CurrentRowName : InitialHorseData->GetRowNames())
	{
		HorseData.Add(*InitialHorseData->FindRow<FHorseDataStruct>(CurrentRowName, "Horse Data Creation...", true));
	}
}

void UBaseGameInstance::SaveGame()
{
}

void UBaseGameInstance::UpdateHorseData(int HorseID)
{
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == HorseID)
		{
			SaveGameRef->HorseData[i] = HorseData[i];
			break;
		}
	}
}
