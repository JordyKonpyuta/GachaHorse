// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllStructs.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

struct FEquipDataStruct;
struct FHorseDataStruct;
class UHorseGameSave;
/**
 * 
 */
UCLASS()
class GACHAHORSE_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// UPROPERTIES
public:
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName = "You";

	// HORSES
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> InitialHorseData;
	UPROPERTY(BlueprintReadWrite)
	TArray<FHorseDataStruct> HorseData;

	UPROPERTY()
	FHorseDataStruct ChosenHorseData = {
		true, 25, 6, 1, 2, "TestWhorse",
		nullptr, {5,7,8,10,11,13},
		{6,6,7,7,8,9}, {2,4,7,12,12,18},
		nullptr, nullptr};

	// EQUIPMENTS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> InitialEquipmentData;
	UPROPERTY(BlueprintReadWrite)
	TArray<FEquipDataStruct> EquipData;

	UPROPERTY()
	FEquipDataStruct ChosenEquipData = {
		true, 6, 2, 0, "TestSaddle",
		nullptr, {2,3,5,7,8,12},
		{3,4,5,6,9,13}, {5,5,6,6,7,9},
		nullptr};

private:
	UPROPERTY()
	FString SaveName = "OnlySave";
	
	UPROPERTY()
	TObjectPtr<UHorseGameSave> SaveGameRef;

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	UBaseGameInstance();

private:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	UFUNCTION()
	void CheckSaves();
	
	// ==========================
	// ==   GACHA! GAMBLING!   ==
	// ==========================

	UFUNCTION()
	TArray<int> CalculateHorseGains(bool bTenSummons);
	
	// =========================
	// ==     Actual Game     ==
	// =========================

	UFUNCTION(BlueprintCallable)
	void StartGame(TSoftObjectPtr<UWorld> WorldToLoad, int HorseID, int EquipmentID);
	
	// =========================
	// ==   Saves Functions   ==
	// =========================

	UFUNCTION()
	void InitializeFirstSave();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void UpdateHorseData(int HorseID);
};
