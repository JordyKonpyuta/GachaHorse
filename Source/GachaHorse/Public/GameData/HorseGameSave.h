// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllStructs.h"
#include "GameFramework/SaveGame.h"
#include "HorseGameSave.generated.h"

struct FWorldMapDataStruct;
struct FEquipDataStruct;
struct FHorseDataStruct;
/**
 * 
 */
UCLASS()
class GACHAHORSE_API UHorseGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FHorseDataStruct> HorseData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHorseDataStruct ChosenHorseData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEquipDataStruct> EquipData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FEquipDataStruct ChosenEquipData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWorldMapDataStruct> LevelData;

	// MONEYS
	UPROPERTY()
	int SummonMoney = 0;
	UPROPERTY()
	int ScrapMoney = 0;

	// MISSIONS
	UPROPERTY()
	FDateTime MissionNextResetRef;
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> TrackMissionTarget;
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	int RankMissionTarget;
	UPROPERTY(Blueprintable, BlueprintReadOnly)
	int HorseIDMissionTarget;
	
};
