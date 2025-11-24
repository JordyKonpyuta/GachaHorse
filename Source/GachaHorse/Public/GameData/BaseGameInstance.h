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
	// =========================
	// ==   All Data Values   ==
	// =========================
	
	UPROPERTY(BlueprintReadWrite)
	FString PlayerName = "You";

	// HORSES
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> InitialHorseData;
	UPROPERTY(BlueprintReadOnly)
	TArray<FHorseDataStruct> HorseData;

	UPROPERTY(BlueprintReadWrite)
	FHorseDataStruct ChosenHorseData = {
		true, 25, 6, 1, 2, "TestHorse",
		nullptr, {5,7,8,10,11,25},
		{6,6,7,7,8,25}, {2,4,7,12,12,25},
		nullptr, nullptr};


	UPROPERTY(BlueprintReadWrite)
	FHorseDataStruct ViewedHorseData = {
		true, 25, 6, 1, 2, "TestHorse",
		nullptr, {5,7,8,10,11,25},
		{6,6,7,7,8,25}, {2,4,7,12,12,25},
		nullptr, nullptr};

	// EQUIPMENTS
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> InitialEquipmentData;
	UPROPERTY(BlueprintReadWrite)
	TArray<FEquipDataStruct> EquipData;

	UPROPERTY(BlueprintReadWrite)
	FEquipDataStruct ChosenEquipData = {
		true, 6, 2, 0, "TestSaddle",
		nullptr, {2,3,5,7,8,0},
		{3,4,5,6,9,0}, {5,5,6,6,7,0},
		nullptr};

	UPROPERTY(BlueprintReadWrite)
	FEquipDataStruct ViewedEquipData = {
		true, 6, 2, 0, "TestSaddle",
		nullptr, {2,3,5,7,8,0},
		{3,4,5,6,9,0}, {5,5,6,6,7,0},
		nullptr};

	// Levels
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> InitialLevelData;
	UPROPERTY(BlueprintReadWrite)
	TArray<FWorldMapDataStruct> LevelData;
	UPROPERTY(BlueprintReadWrite)
	int LevelSelected = 0;
	
	// =========================
	// == All Missions Values ==
	// =========================

	FDateTime MissionNextResetRef;
	bool MissionUnavailable = false;

	// TRACK MISSION
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> TrackMissionTarget;

	// RANK MISSION
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	int RankMissionTarget = 101;

	// HORSE MISSION
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	int HorseIDMissionTarget = 0;
	
	// =========================
	// ==  Other Game Values  ==
	// =========================

	UPROPERTY()
	bool bIsTrainingMode = false;

private:
	UPROPERTY()
	FString SaveName = "OnlySave";
	
	UPROPERTY()
	TObjectPtr<UHorseGameSave> SaveGameRef;
	
	// =========================
	// ==        Money        ==
	// =========================

	UPROPERTY()
	int SummonMoney = 0;

	UPROPERTY()
	int ScrapMoney = 0;

	// UFUNCTIONS
public:
	// ==========================
	// ==    Base Functions    ==
	// ==========================
	
	UBaseGameInstance();
	
	// =========================
	// ==        Money        ==
	// =========================

	UFUNCTION(Blueprintable, BlueprintCallable, BlueprintPure)
	int GetMoney();
	UFUNCTION()
	void AddMoney(int MoneyAdded);
	UFUNCTION()
	void SetMoney(int NewMoney);
	
	UFUNCTION(Blueprintable, BlueprintCallable, BlueprintPure)
	int GetScrap();
	UFUNCTION()
	void AddScrap(int MoneyAdded);
	UFUNCTION()
	void SetScrap(int NewMoney);
	
	// ==========================
	// ==     Money Widget     ==
	// ==========================

	UFUNCTION(BlueprintNativeEvent, Blueprintable)
	void UpdateMoney();
	
	// ==========================
	// ==   Prepare Missions   ==
	// ==========================

	UFUNCTION()
	void PrepareMissions();
	
	// =========================
	// == Horses & Equip Data ==
	// =========================

	UFUNCTION()
	void ObtainedHorse(int HorseID);
	UFUNCTION()
	void ObtainedEquip(int EquipID);

	UFUNCTION(BlueprintCallable, Blueprintable)
	void SelectHorse(int HorseID);
	UFUNCTION()
	void CheckAvailableHorse(bool bCheckingLeft);
	UFUNCTION(BlueprintCallable, Blueprintable)
	void EquipHorse();

	UFUNCTION(BlueprintCallable, Blueprintable)
	void SelectEquip(int EquipID);
	UFUNCTION()
	void CheckAvailableEquip(bool bCheckingLeft);
	UFUNCTION(BlueprintCallable, Blueprintable)
	void EquipEquips();

	UFUNCTION()
	void LevelUpHorse();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LevelUpHorseVisual();
	UFUNCTION()
	void LevelUpEquip();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LevelUpEquipVisual();

private:
	// ==========================
	// ==    Base Functions    ==
	// ==========================

	virtual void Init() override;

	UFUNCTION()
	void CheckSaves();
	
	// =========================
	// ==     Actual Game     ==
	// =========================

	UFUNCTION(BlueprintCallable)
	void StartGame(TSoftObjectPtr<UWorld> WorldToLoad, bool bGoesToTrainingMode);
	
	// =========================
	// ==   Saves Functions   ==
	// =========================

	UFUNCTION()
	void InitializeFirstSave();

	UFUNCTION()
	void SaveGame();

	UFUNCTION()
	void SaveHorseData();

	UFUNCTION()
	void SaveEquipData();

	UFUNCTION()
	void SaveWorldData();

	UFUNCTION()
	void SaveMoneyData();

	UFUNCTION()
	void SaveMissionData();

	UFUNCTION()
	void LoadData();
	
	// =========================
	// ==   Saves Integrity   ==
	// =========================

	UFUNCTION()
	void CheckFileIntegrity();

	UFUNCTION()
	void CheckHorseIntegrity();

	UFUNCTION()
	void CheckEquipIntegrity();

	UFUNCTION()
	void CheckWorldIntegrity();

	UFUNCTION()
	void CheckMoneyIntegrity();

	UFUNCTION()
	void CheckMissionIntegrity();
};
