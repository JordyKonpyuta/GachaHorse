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

	// ==========================
	// ==     Title Screen     ==
	// ==========================

void UBaseGameInstance::TitleScreenLoaded()
{
	bTitleScreenAlreadyAppeared = true;
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
	SaveGame();
	UpdateMoney();
}

void UBaseGameInstance::SetMoney(int NewMoney)
{
	SummonMoney = NewMoney;
	
	SaveGameRef->SummonMoney = SummonMoney;
	SaveGame();
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
	SaveGame();
	UpdateMoney();
}

void UBaseGameInstance::SetScrap(int NewMoney)
{
	ScrapMoney = NewMoney;
	
	SaveGameRef->ScrapMoney = ScrapMoney;
	SaveGame();
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
	SaveGame();
}

	// =========================
	// ==     Actual Game     ==
	// =========================

void UBaseGameInstance::StartGame(TSoftObjectPtr<UWorld> WorldToLoad, bool bGoesToTrainingMode)
{
	bIsTrainingMode = bGoesToTrainingMode;

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
			AllHorsesPulled.Add(HorseData[i]);
			if (!HorseData[i].bHorsePossessed)
				HorseData[i].bHorsePossessed = true;
			else
			{
				if (HorseData[i].Level >= 6)
					AddMoney(50);
				else
					HorseData[i].ShardNumber += 1;
			}
			
			if (SaveGameRef->IsValidLowLevelFast())
				SaveGameRef->HorseData[i] = HorseData[i];
				
			break;
		}
	}
	SaveGame();
}

void UBaseGameInstance::ObtainedEquip(int EquipID)
{
	for (int i = 0; i < EquipData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == EquipID)
		{
			AllEquipsPulled.Add(EquipData[i]);
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
	SaveGame();
}

void UBaseGameInstance::SelectHorse(int HorseID)
{
	ViewedHorseData = HorseData[HorseID];
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->SelectHorseWidget(ViewedHorseData);
}

void UBaseGameInstance::CheckAvailableHorse(bool bCheckingLeft)
{
	int CurHorseID = 0;
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == ViewedHorseData.HorseID)
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

void UBaseGameInstance::EquipHorse()
{
	ChosenHorseData = ViewedHorseData;
	SaveGame();
}

void UBaseGameInstance::SelectEquip(int EquipID)
{
	ViewedEquipData = EquipData[EquipID];
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->SelectEquipWidget(ViewedEquipData);
}

void UBaseGameInstance::CheckAvailableEquip(bool bCheckingLeft)
{
	int CurEquipID = 0;
	for (int i = 0; i < EquipData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == ViewedEquipData.EquipmentID)
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

void UBaseGameInstance::EquipEquips()
{
	ChosenEquipData = ViewedEquipData;
	SaveGame();
}

void UBaseGameInstance::LevelUpHorse()
{
	for (int i = 0; i < HorseData.Num(); i++)
	{
		if (HorseData[i].HorseID == ViewedHorseData.HorseID)
		{
			int Price = FMath::Pow(2.0, HorseData[i].Level - 1);
			
			if (HorseData[i].Level >= 6 || HorseData[i].ShardNumber < Price)
				return;
			
			HorseData[i].ShardNumber -= FMath::Pow(2.0f, HorseData[i].Level - 1);
			HorseData[i].Level += 1;
			ViewedHorseData = HorseData[i];

			if (HorseData[i].Level >= 6)
			{
				AddMoney(HorseData[i].ShardNumber * 50);
				HorseData[i].ShardNumber = 0;
			}

			if (ViewedHorseData.HorseID == ChosenHorseData.HorseID)
				ChosenHorseData = ViewedHorseData;

			SaveGame();
			LevelUpHorseVisual();
			break;
		}
	}
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->SelectHorseWidget(ViewedHorseData);
}

void UBaseGameInstance::LevelUpHorseVisual_Implementation()
{
}

void UBaseGameInstance::LevelUpEquip()
{
	for (int i = 0; i < EquipData.Num(); i++)
	{
		if (EquipData[i].EquipmentID == ViewedEquipData.EquipmentID)
		{
			if (EquipData[i].Level >= 6)
				return;
			
			int BaseCost = 0;
			if (EquipData[i].Rarity == 0)
				BaseCost = 2;
			else if (EquipData[i].Rarity == 1)
				BaseCost = 5;
			else
				BaseCost = 25;

			if (GetScrap() < BaseCost * FMath::Pow(2.0f, EquipData[i].Level - 1))
				return;
			AddScrap(-(BaseCost * FMath::Pow(2.0f, EquipData[i].Level - 1)));
			EquipData[i].Level += 1;
			ViewedEquipData = EquipData[i];

			if (ViewedEquipData.EquipmentID == ChosenEquipData.EquipmentID)
				ChosenEquipData = ViewedEquipData;

			LevelUpEquipVisual();

			SaveGame();
			break;
		}
	}
	Cast<AMainMenuGamemode>(GetWorld()->GetAuthGameMode())->SelectEquipWidget(ViewedEquipData);
}

void UBaseGameInstance::LevelUpEquipVisual_Implementation()
{
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
	HorseData[0].bHorsePossessed = true;
	ChosenHorseData = HorseData[0];
	ViewedHorseData = ChosenHorseData;
	
	for (FName CurrentRowName : InitialEquipmentData->GetRowNames())
	{
		EquipData.Add(*InitialEquipmentData->FindRow<FEquipDataStruct>(CurrentRowName, "Horse Data Creation...", true));
	}
	EquipData[0].bEquipmentPossessed = true;
	ChosenEquipData = EquipData[0];
	ViewedEquipData = ChosenEquipData;
	
	for (FName CurrentRowName : InitialLevelData->GetRowNames())
	{
		LevelData.Add(*InitialLevelData->FindRow<FWorldMapDataStruct>(CurrentRowName, "Horse Data Creation...", true));
	}
	SummonMoney = 5000;

	SaveGame();
}

void UBaseGameInstance::SaveGame()
{
	SaveHorseData();
	SaveEquipData();
	SaveWorldData();
	SaveMoneyData();
	SaveMissionData();
	SaveOtherData();
	UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveName, 0);
}

void UBaseGameInstance::SaveHorseData() const
{
	SaveGameRef->HorseData = HorseData;
	SaveGameRef->ChosenHorseData = ChosenHorseData;
}

void UBaseGameInstance::SaveEquipData() const
{
	SaveGameRef->EquipData = EquipData;
	SaveGameRef->ChosenEquipData = ChosenEquipData;
}

void UBaseGameInstance::SaveWorldData() const
{
	SaveGameRef->LevelData = LevelData;
}

void UBaseGameInstance::SaveMoneyData() const
{
	 SaveGameRef->SummonMoney = SummonMoney;
	 SaveGameRef->ScrapMoney = ScrapMoney;
}

void UBaseGameInstance::SaveMissionData() const
{
	 SaveGameRef->MissionNextResetRef = MissionNextResetRef;
	 SaveGameRef->TrackMissionTarget = TrackMissionTarget;
	 SaveGameRef->RankMissionTarget = RankMissionTarget;
	 SaveGameRef->HorseIDMissionTarget = HorseIDMissionTarget;
}

void UBaseGameInstance::SaveOtherData()
{
	SaveGameRef->PlayerName = PlayerName;
}

void UBaseGameInstance::LoadData()
{
	if (!SaveGameRef->IsValidLowLevelFast())
		return;
	
	HorseData = SaveGameRef->HorseData;
	EquipData = SaveGameRef->EquipData;
	LevelData = SaveGameRef->LevelData;

	ChosenHorseData = SaveGameRef->ChosenHorseData;
	ViewedHorseData = ChosenHorseData;
	ChosenEquipData = SaveGameRef->ChosenEquipData;
	ViewedEquipData = ChosenEquipData;
	
	SummonMoney = SaveGameRef->SummonMoney;
	ScrapMoney = SaveGameRef->ScrapMoney;
	
	MissionNextResetRef = SaveGameRef->MissionNextResetRef;
	TrackMissionTarget = SaveGameRef->TrackMissionTarget;
	RankMissionTarget = SaveGameRef->RankMissionTarget;
	HorseIDMissionTarget = SaveGameRef->HorseIDMissionTarget;

	PlayerName = SaveGameRef->PlayerName;

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
	UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveName, 0);
}

void UBaseGameInstance::CheckHorseIntegrity()
{
	if (SaveGameRef->HorseData.Num() == InitialHorseData->GetRowNames().Num())
		return;

	GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Red, "HORSES MISSING DETECTED");

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

	for (FHorseDataStruct CurHorse: HorseData)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f, FColor::Blue, CurHorse.HorseName);
	}

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
