// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/BaseGameInstance.h"

#include "GameData/AllStructs.h"
#include "GameData/HorseGameSave.h"
#include "GameData/MainMenuGamemode.h"
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

int UBaseGameInstance::GetMoney()
{
	return SummonMoney;
}

void UBaseGameInstance::AddMoney(int MoneyAdded)
{
	SummonMoney += MoneyAdded;
	
	SaveGameRef->SummonMoney = SummonMoney;
	SaveMoneyData();
	UpdateMoney();
}

void UBaseGameInstance::SetMoney(int NewMoney)
{
	SummonMoney = NewMoney;
	
	SaveGameRef->SummonMoney = SummonMoney;
	SaveMoneyData();
	UpdateMoney();
}

int UBaseGameInstance::GetScrap()
{
	return ScrapMoney;
}

void UBaseGameInstance::AddScrap(int MoneyAdded)
{
	ScrapMoney += MoneyAdded;
	
	SaveGameRef->ScrapMoney = ScrapMoney;
	SaveMoneyData();
	UpdateMoney();
}

void UBaseGameInstance::SetScrap(int NewMoney)
{
	ScrapMoney = NewMoney;
	
	SaveGameRef->ScrapMoney = ScrapMoney;
	SaveMoneyData();
	UpdateMoney();
}
	
	// ==========================
	// ==     Money Widget     ==
	// ==========================

void UBaseGameInstance::UpdateMoney_Implementation()
{
}

	// ==========================
	// ==   Prepare Missions   ==
	// ==========================

void UBaseGameInstance::PrepareMissions()
{
	// MISSION : LEVEL
	int LevelTarget = FMath::RandRange(0, LevelData.Num() - 1);
	TrackMissionTarget = LevelData[LevelTarget].WorldToLoad;

	// MISSION : RANK
	int TargetDifficulty = FMath::RandRange(0, 10);
	
	if (TargetDifficulty == 0)
		RankMissionTarget = FMath::RandRange(1, 5);
	else if (TargetDifficulty == 1)
		RankMissionTarget = FMath::RandRange(6, 15);
	else if (TargetDifficulty == 2 || TargetDifficulty == 3)
		RankMissionTarget = FMath::RandRange(16, 30);
	else if (4 <= TargetDifficulty && TargetDifficulty <= 6)
		RankMissionTarget = FMath::RandRange(31, 70);
	else if (TargetDifficulty == 7 || TargetDifficulty == 8)
		RankMissionTarget = FMath::RandRange(71, 85);
	else if (TargetDifficulty == 9)
		RankMissionTarget = FMath::RandRange(86, 95);
	else
		RankMissionTarget = FMath::RandRange(96, 101);

	// MISSION : HORSE
	HorseIDMissionTarget = FMath::RandRange(0, HorseData.Num() - 1);
	

	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->DisplayMissionsWidget(LevelData[LevelSelected].LevelName, RankMissionTarget, HorseData[HorseIDMissionTarget].HorseName);
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
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == HorseID)
		{
			if (!HorseData[i].bHorsePossessed)
				HorseData[i].bHorsePossessed = true;
			else
				HorseData[i].ShardNumber += 1;
			
			if (SaveGameRef->IsValidLowLevelFast())
				SaveGameRef->HorseData[i] = HorseData[i];
				
			break;
		}
	}
}

void UBaseGameInstance::ObtainedEquip(int EquipID)
{
	for (int i = 0; i < EquipData.Num(); i++)
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
			if (SaveGameRef->IsValidLowLevelFast())
				SaveGameRef->EquipData[i] = EquipData[i];
			break;
		}
	}
}

void UBaseGameInstance::SelectHorse(int HorseID)
{
	ChosenHorseData = HorseData[HorseID];
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->SelectHorseWidget(ChosenHorseData);
}

void UBaseGameInstance::CheckAvailableHorse(bool bCheckingLeft)
{
	int CurHorseID = 0;
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == ChosenHorseData.HorseID)
		{
			CurHorseID = i;
			break;
		}
	}

	while (true)
	{
		CurHorseID += bCheckingLeft ? -1 : 1;
		if (CurHorseID < 0)
			CurHorseID = HorseData.Num() - 1;
		else if (CurHorseID >= HorseData.Num())
			CurHorseID = 0;

		if (HorseData[CurHorseID].bHorsePossessed == true)
		{
			SelectHorse(CurHorseID);
			break;
		}
	}
}

void UBaseGameInstance::SelectEquip(int EquipID)
{
	ChosenEquipData = EquipData[EquipID];
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->SelectEquipWidget(ChosenEquipData);
}

void UBaseGameInstance::CheckAvailableEquip(bool bCheckingLeft)
{
	int CurEquipID = 0;
	for (int i = 0; i < EquipData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == ChosenEquipData.EquipmentID)
		{
			CurEquipID = i;
			break;
		}
	}

	while (true)
	{
		CurEquipID += bCheckingLeft ? -1 : 1;
		if (CurEquipID < 0)
			CurEquipID = EquipData.Num() - 1;
		else if (CurEquipID >= EquipData.Num())
			CurEquipID = 0;

		if (EquipData[CurEquipID].bEquipmentPossessed == true)
		{
			SelectEquip(CurEquipID);
			break;
		}
	}
}

void UBaseGameInstance::LevelUpHorse()
{
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == ChosenHorseData.HorseID)
		{
			HorseData[i].ShardNumber -= 2 ^ (HorseData[i].Level - 1);
			HorseData[i].Level += 1;
			ChosenHorseData = HorseData[i];

			SaveHorseData();
			break;
		}
	}
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->LevelUpHorseWidgetUpdate(ChosenHorseData);
}

void UBaseGameInstance::LevelUpEquip()
{
	for (int i = 0; i < EquipData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == ChosenEquipData.EquipmentID)
		{
			int BaseCost = 0;
			if (EquipData[i].Rarity == 0)
				BaseCost = 2;
			else if (EquipData[i].Rarity == 1)
				BaseCost = 5;
			else
				BaseCost = 25;
			AddScrap(BaseCost * (2 ^ (EquipData[i].Level - 1)));
			EquipData[i].Level += 1;
			ChosenEquipData = EquipData[i];

			SaveEquipData();
			break;
		}
	}
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->LevelUpEquipWidgetUpdate(ChosenEquipData);
}

	// =========================
	// ==   Saves Functions   ==
	// =========================

void UBaseGameInstance::CheckSaves()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveName, 0))
	{
		SaveGameRef = Cast<UHorseGameSave>(UGameplayStatics::LoadGameFromSlot(SaveName, 0));
		LoadData();
	}
	else
	{
		SaveGameRef = Cast<UHorseGameSave>(UGameplayStatics::CreateSaveGameObject(UHorseGameSave::StaticClass()));
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
	SaveMoneyData();
	SaveMissionData();
	UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveName, 0);
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

void UBaseGameInstance::SaveMoneyData()
{
	 SaveGameRef->SummonMoney = SummonMoney;
	 SaveGameRef->ScrapMoney = ScrapMoney;
}

void UBaseGameInstance::SaveMissionData()
{
	 SaveGameRef->MissionNextResetRef = MissionNextResetRef;
	 SaveGameRef->TrackMissionTarget = TrackMissionTarget;
	 SaveGameRef->RankMissionTarget = RankMissionTarget;
	 SaveGameRef->HorseIDMissionTarget = HorseIDMissionTarget;
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
	MissionNextResetRef = SaveGameRef->MissionNextResetRef;
	TrackMissionTarget = SaveGameRef->TrackMissionTarget;
	RankMissionTarget = SaveGameRef->RankMissionTarget;
	HorseIDMissionTarget = SaveGameRef->HorseIDMissionTarget;

	CheckFileIntegrity();
}
	
// =========================
// ==   Saves Integrity   ==
// =========================

void UBaseGameInstance::CheckFileIntegrity()
{
	if (!SaveGameRef->IsValidLowLevelFast())
		return;

	CheckHorseIntegrity();
	CheckEquipIntegrity();
	CheckWorldIntegrity();
	CheckMoneyIntegrity();
	CheckMissionIntegrity();
}

void UBaseGameInstance::CheckHorseIntegrity()
{
	if (SaveGameRef->HorseData.Num() == InitialHorseData->GetRowNames().Num())
		return;

	TArray<FHorseDataStruct> TempHorseData;
	bool bCheckIfExists = false;
	
	for (FName CurrentRowName : InitialHorseData->GetRowNames())
	{
		bCheckIfExists = false;
		FString CurHorseName = *InitialHorseData->FindRow<FHorseDataStruct>(CurrentRowName, "Horse Data Analysis...", true)->HorseName;
		for (FHorseDataStruct CurDataAnalyzed : SaveGameRef->HorseData)
		{
			if (CurHorseName == CurDataAnalyzed.HorseName)
			{
				TempHorseData.Add(CurDataAnalyzed);
				bCheckIfExists = true;
				break;
			}
		}
		if (!bCheckIfExists)
		{
			TempHorseData.Add(*InitialHorseData->FindRow<FHorseDataStruct>(CurrentRowName, "Horse Data Missing! Creation...", true));
		}
	}

	HorseData = TempHorseData;
	SaveHorseData();
}

void UBaseGameInstance::CheckEquipIntegrity()
{
	if (SaveGameRef->EquipData.Num() == InitialEquipmentData->GetRowNames().Num())
		return;

	TArray<FEquipDataStruct> TempEquipData;
	bool bCheckIfExists = false;
	
	for (FName CurrentRowName : InitialEquipmentData->GetRowNames())
	{
		bCheckIfExists = false;
		FString CurEquipName = *InitialEquipmentData->FindRow<FEquipDataStruct>(CurrentRowName, "Equip Data Analysis...", true)->EquipmentName;
		for (FEquipDataStruct CurDataAnalyzed : SaveGameRef->EquipData)
		{
			if (CurEquipName == CurDataAnalyzed.EquipmentName)
			{
				TempEquipData.Add(CurDataAnalyzed);
				bCheckIfExists = true;
				break;
			}
		}
		if (!bCheckIfExists)
		{
			TempEquipData.Add(*InitialEquipmentData->FindRow<FEquipDataStruct>(CurrentRowName, "Equip Data Missing! Creation...", true));
		}
	}

	EquipData = TempEquipData;
	SaveEquipData();
}

void UBaseGameInstance::CheckWorldIntegrity()
{
	if (SaveGameRef->LevelData.Num() == InitialLevelData->GetRowNames().Num())
		return;

	TArray<FWorldMapDataStruct> TempWorldData;
	bool bCheckIfExists = false;
	
	for (FName CurrentRowName : InitialLevelData->GetRowNames())
	{
		bCheckIfExists = false;
		FString CurLevelName = *InitialLevelData->FindRow<FWorldMapDataStruct>(CurrentRowName, "World Data Analysis...", true)->LevelName;
		for (FWorldMapDataStruct CurDataAnalyzed : SaveGameRef->LevelData)
		{
			if (CurLevelName == CurDataAnalyzed.LevelName)
			{
				TempWorldData.Add(CurDataAnalyzed);
				bCheckIfExists = true;
				break;
			}
		}
		if (!bCheckIfExists)
		{
			TempWorldData.Add(*InitialLevelData->FindRow<FWorldMapDataStruct>(CurrentRowName, "World Data Missing! Creation...", true));
		}
	}

	LevelData = TempWorldData;
	SaveWorldData();
}

void UBaseGameInstance::CheckMoneyIntegrity()
{
	if (SummonMoney < 0)
		SummonMoney = 0;

	if (ScrapMoney < 0)
		ScrapMoney = 0;

	SaveMoneyData();
}

void UBaseGameInstance::CheckMissionIntegrity()
{
	if (MissionNextResetRef.GetYear() < 1900)
		MissionNextResetRef = FDateTime::Now();

	
}
