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

void UBaseGameInstance::Init()
{
	Super::Init();

	CheckSaves();
}
	
	// =========================
	// ==     Money Gains     ==
	// =========================

void UBaseGameInstance::AddMoney(int MoneyAdded)
{
	SummonMoney += MoneyAdded;
}

void UBaseGameInstance::SetMoney(int NewMoney)
{
	SummonMoney = NewMoney;
}

void UBaseGameInstance::AddScrap(int MoneyAdded)
{
	ScrapMoney += MoneyAdded;
}

void UBaseGameInstance::SetScrap(int NewMoney)
{
	ScrapMoney = NewMoney;
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

	for (int i = 0; i < LevelData.Num(); i++)
	{
		if (LevelData[i].WorldToLoad == WorldToLoad)
		{
			LevelSelected = i;
			UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),WorldToLoad);
			break;
		}
	}
}

	// =========================
	// ==     Horses Data     ==
	// =========================

void UBaseGameInstance::ObtainedHorse(int HorseID)
{
	GEngine->AddOnScreenDebugMessage(-1,50.f, FColor::Red, "A");
	for (int i = 0; i < HorseData.Num(); i++)
	{
		GEngine->AddOnScreenDebugMessage(-1,50.f, FColor::Red, "B");
		
		if (HorseData[i].HorseID == HorseID)
		{
			GEngine->AddOnScreenDebugMessage(-1,50.f, FColor::Red, "C");
			
			if (!HorseData[i].bHorsePossessed)
				HorseData[i].bHorsePossessed = true;
			else
				HorseData[i].ShardNumber += 1;
			
			if (SaveGameRef->IsValidLowLevelFast())
				SaveGameRef->HorseData[i] = HorseData[i];
				
			break;
		}
		
	}
	GEngine->AddOnScreenDebugMessage(-1,50.f, FColor::Red, "D");
}

void UBaseGameInstance::ObtainedEquip(int EquipID)
{
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == EquipID)
		{
			if (!EquipData[i].bEquipmentPossessed)
				EquipData[i].bEquipmentPossessed = true;
			else
			{
				if (EquipData[i].Rarity == 0)
					ScrapMoney += 1;
				else if (EquipData[i].Rarity == 1)
					ScrapMoney += 3;
				else
					ScrapMoney += 10;
			}
			SaveGameRef->EquipData[i] = EquipData[i];
			break;
		}
	}
}

	// =========================
	// ==   Saves Functions   ==
	// =========================

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

void UBaseGameInstance::InitializeFirstSave()
{
	for (FName CurrentRowName : InitialHorseData->GetRowNames())
	{
		HorseData.Add(*InitialHorseData->FindRow<FHorseDataStruct>(CurrentRowName, "Horse Data Creation...", true));
	}
	
	for (FName CurrentRowName : InitialEquipmentData->GetRowNames())
	{
		EquipData.Add(*InitialEquipmentData->FindRow<FEquipDataStruct>(CurrentRowName, "Horse Data Creation...", true));
	}
	
	for (FName CurrentRowName : InitialLevelData->GetRowNames())
	{
		LevelData.Add(*InitialLevelData->FindRow<FWorldMapDataStruct>(CurrentRowName, "Horse Data Creation...", true));
	}

	SaveGame();
}

void UBaseGameInstance::SaveGame()
{
	SaveHorseData();
	SaveEquipData();
	SaveWorldData();
}

void UBaseGameInstance::SaveHorseData()
{
	SaveGameRef->HorseData = HorseData;
	SaveGameRef->SummonMoney = SummonMoney;
}

void UBaseGameInstance::SaveEquipData()
{
	SaveGameRef->EquipData = EquipData;
	SaveGameRef->ScrapMoney = ScrapMoney;
}

void UBaseGameInstance::SaveWorldData()
{
	SaveGameRef->LevelData = LevelData;
}

void UBaseGameInstance::LoadData()
{
	if (!SaveGameRef->IsValidLowLevelFast())
		return;
	
	HorseData = SaveGameRef->HorseData;
	EquipData = SaveGameRef->EquipData;
	LevelData = SaveGameRef->LevelData;
	SummonMoney = SaveGameRef->SummonMoney;
	ScrapMoney = SaveGameRef->ScrapMoney;
}
