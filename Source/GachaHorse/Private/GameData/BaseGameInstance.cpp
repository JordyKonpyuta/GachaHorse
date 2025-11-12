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
