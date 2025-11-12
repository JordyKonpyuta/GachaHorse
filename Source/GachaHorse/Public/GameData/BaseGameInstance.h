// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable> InitialHorseData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHorseDataStruct> HorseData;

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
