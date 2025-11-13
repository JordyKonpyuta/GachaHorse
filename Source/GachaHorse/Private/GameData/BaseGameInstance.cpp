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
	// ==     Horses Data     ==
	// =========================

void UBaseGameInstance::ObtainedHorse(int HorseID)
{
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == HorseID)
		{
			if (!HorseData[i].bHorsePossessed)
				HorseData[i].bHorsePossessed = true;
			else
				HorseData[i].ShardNumber += 1;
			SaveGameRef->HorseData[i] = HorseData[i];
			break;
		}
	}
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
